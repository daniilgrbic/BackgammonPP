#include <catch2/catch_test_macros.hpp>

#include "../engine/backgammon.h"

class BackgammonTestHelper : public Backgammon {
public:
    void setBoard(const BoardState& board) {
        m_board = board;
    }

    void setCurrentRoll(PlayerColor onRoll, const std::vector<int>& dice) {
        m_currentRoll = {onRoll, dice};
    }
};

using checkers = std::vector<std::pair<int, int>>;

TEST_CASE("Backgammon initial BoardState") {

    SECTION("Both players have CHECKER_COUNT checkers that are all on board") {
        // Arrange
        auto game = Backgammon();
        auto whiteCheckers = 0;
        auto blackCheckers = 0;
        auto board = game.board();

        // Act
        for (auto i = 1; i <= NUMBER_OF_POINTS; ++i) {
            if (board.point(i).owner()) {
                auto owner = board.point(i).owner().value();
                auto& checkers = owner == PlayerColor::WHITE ? whiteCheckers : blackCheckers;
                checkers += board.point(i).count();
            }
        }

        // Assert
        REQUIRE_FALSE(board.bar(PlayerColor::WHITE));
        REQUIRE_FALSE(board.bar(PlayerColor::BLACK));
        REQUIRE_FALSE(board.off(PlayerColor::WHITE));
        REQUIRE_FALSE(board.off(PlayerColor::BLACK));
        REQUIRE(whiteCheckers == game.CHECKERS_COUNT);
        REQUIRE(blackCheckers == game.CHECKERS_COUNT);
    }

    SECTION("Starting points are symmetric", "[property]") {
        // Arrange
        auto game = Backgammon();
        auto board = game.board();

        // Act & Assert
        for (auto i = 1; i <= NUMBER_OF_POINTS; ++i) {
            auto& point = board.point(i);
            if (point.owner()) {
                auto owner = board.point(i).owner().value();
                auto opponent = owner == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE;
                auto& symmetricPoint = board.point(NUMBER_OF_POINTS + 1 - i);
                REQUIRE(symmetricPoint.owner().has_value());
                REQUIRE(symmetricPoint.owner().value() == opponent);
                REQUIRE(point.count() == symmetricPoint.count());
            }
        }
    }
}

TEST_CASE("Backgammon initial Roll") {

    SECTION("Initial roll should never be doubles") {
        // Arrange
        auto game = Backgammon();

        // Act
        auto dice = game.currentRoll().dice();

        // Assert
        REQUIRE(dice.size() == 2);
        REQUIRE(dice[0] != dice[1]);
    }

    SECTION("Inititial roll is 5-2 and WHITE is on turn, there is no hitting") {
        // Arrange
        auto onRoll = PlayerColor::WHITE;
        std::vector<int> dice = { 5, 2 };
        auto game = BackgammonTestHelper();
        bool isHit = false;
        auto turns = game.generateLegalTurns();

        // Act
        game.setCurrentRoll(onRoll, dice);
        for (const auto& turn : turns)
            for (const auto& move : turn.m_moves)
                isHit |= move.m_isHit;

        // Assert
        REQUIRE_FALSE(isHit);
    }

    SECTION("Inititial roll is 4-3, number of turns should be the same for both players") {
        // Arrange
        std::vector<int> dice = { 4, 3 };
        auto gameWhite = BackgammonTestHelper();
        auto gameBlack = BackgammonTestHelper();

        // Act
        gameWhite.setCurrentRoll(PlayerColor::WHITE, dice);
        gameBlack.setCurrentRoll(PlayerColor::BLACK, dice);
        auto whiteTurns = gameWhite.generateLegalTurns();
        auto blackTurns = gameBlack.generateLegalTurns();

        // Assert
        REQUIRE(whiteTurns.size() == blackTurns.size());
    }

    SECTION("Inititial roll is 5-2, number of unique turns is 8") {
        // Arrange
        std::vector<int> dice = { 5, 2 };
        auto game = BackgammonTestHelper();
        auto expectedNumberOfTurns = 8;

        // Act
        game.setCurrentRoll(PlayerColor::WHITE, dice);
        auto turns = Turn::unique(game.generateLegalTurns());

        // Assert
        REQUIRE(turns.size() == 8);
    }
}

TEST_CASE("Backgammon mid game legal turns") {

    SECTION("If WHITE has 2 and 5 closed and BLACK roll 2-5 BLACK can't make a turn") {
        // Arrange
        checkers whitePoints { {2, 10}, {5, 5} };
        checkers blackPoints { {24, 14} };
        auto whiteBar = 0;
        auto blackBar = 1;
        auto whiteOff = 0;
        auto blackOff = 0;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto onRoll = PlayerColor::BLACK;
        std::vector<int> dice { 2, 5 };
        auto game = BackgammonTestHelper();

        // Act
        game.setBoard(board);
        game.setCurrentRoll(onRoll, dice);
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        REQUIRE(turns[0].m_moves.empty());
    }

    SECTION("If BLACK home board is closed WHITE can't make a turn") {
        // Arrange
        checkers whitePoints { {1, 2}, {2, 3}, {3, 3}, {4, 1}, {5, 3}, {6, 2} };
        checkers blackPoints { {24, 2}, {23, 2}, {22, 3}, {21, 3}, {20, 2}, {19, 3} };
        auto whiteBar = 1;
        auto blackBar = 0;
        auto whiteOff = 0;
        auto blackOff = 0;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto game = BackgammonTestHelper();
        auto onRoll = PlayerColor::WHITE;

        // Act & Assert
        game.setBoard(board);
        for (int i = 1; i <= 6; ++i)
            for (int j = 1; j <= 6; ++j) {
                std::vector<int> dice { i, j };
                game.setCurrentRoll(onRoll, dice);
                auto turns = game.generateLegalTurns();
                REQUIRE(turns.size() == 1);
                REQUIRE(turns[0].m_moves.empty());
            }
    }

    SECTION("If WHITE has 2 checkers on the bar and rolls 3-6 and 6 is blocked then the only move is bar/22") {
        // Arrange
        checkers whitePoints { {1, 13} };
        checkers blackPoints { {19, 14} };
        auto whiteBar = 2;
        auto blackBar = 0;
        auto whiteOff = 0;
        auto blackOff = 0;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto onRoll = PlayerColor::WHITE;
        std::vector<int> dice { 3, 6 };
        auto game = BackgammonTestHelper();
        auto expectedMove = Move(onRoll, SpecialPosition::BAR, 22);

        // Act
        game.setBoard(board);
        game.setCurrentRoll(onRoll, dice);
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        REQUIRE(turns[0].m_moves.size() == 1);
        REQUIRE(turns[0].m_moves[0] == expectedMove);
    }

    SECTION("If WHITE has 1 checker on bar, all other checkers on points <= 5, 19 is blocked and rolls 1-6 the only move is hit and run") {
        // Arrange
        checkers whitePoints { {5, 14} };
        checkers blackPoints { {24, 1}, {19, 14} };
        auto whiteBar = 1;
        auto blackBar = 0;
        auto whiteOff = 0;
        auto blackOff = 0;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto onRoll = PlayerColor::WHITE;
        std::vector<int> dice { 1, 6 };
        auto game = BackgammonTestHelper();
        std::vector<Move> expectedMoves = { { onRoll, SpecialPosition::BAR, 24, true }, { onRoll, 24, 18 } };

        // Act
        game.setBoard(board);
        game.setCurrentRoll(onRoll, dice);
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        REQUIRE(turns[0].m_moves == expectedMoves);
    }
}

TEST_CASE("Backgammon bearing off legal turns") {

    SECTION("General case when WHITE bears off - number of turns") {
        // Arrange
        checkers whitePoints { {1, 1}, {2, 1}, {3, 2}, {4, 4}, {5, 2}, {6, 3} };
        checkers blackPoints { {24, 15} };
        auto whiteBar = 0;
        auto blackBar = 0;
        auto whiteOff = 2;
        auto blackOff = 0;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto onRoll = PlayerColor::WHITE;
        std::vector<int> dice { 5, 2 };
        auto game = BackgammonTestHelper();
        auto expectedNumberOfTurns = 10;

        // Act
        game.setBoard(board);
        game.setCurrentRoll(onRoll, dice);
        auto turns = Turn::unique(game.generateLegalTurns());

        // Assert
        REQUIRE(turns.size() == expectedNumberOfTurns);
    }

    SECTION("If BLACK has one checker left on 24 any combination of dice result in only one move - 24/off") {
        // Arrange
        checkers whitePoints { {1, 15} };
        checkers blackPoints { {24, 1} };
        auto whiteBar = 0;
        auto blackBar = 0;
        auto whiteOff = 0;
        auto blackOff = 14;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto onRoll = PlayerColor::BLACK;
        auto expectedMove = Move(onRoll, 24, SpecialPosition::OFF);
        auto game = BackgammonTestHelper();

        // Act & Assert
        game.setBoard(board);
        for (auto i = 1; i <= 6; ++i)
            for (auto j = 1; j <= 6; ++j) {
                game.setCurrentRoll(onRoll, { i, j });
                auto turns = Turn::unique(game.generateLegalTurns());
                REQUIRE(turns.size() == 1);
                REQUIRE(turns[0].m_moves.size() == 1);
                REQUIRE(turns[0].m_moves[0] == expectedMove);
            }
    }
}

TEST_CASE("Backgammon is game finished") {

    SECTION("Initial game is not finished yet") {
        // Arrange
        auto game = BackgammonTestHelper();

        // Act
        auto isFinishedWhite = game.isFinished(PlayerColor::WHITE);
        auto isFinishedBlack = game.isFinished(PlayerColor::BLACK);

        // Assert
        REQUIRE_FALSE(isFinishedWhite);
        REQUIRE_FALSE(isFinishedBlack);
    }

    SECTION("WHITE has finished game") {
        // Arrange
        checkers whitePoints { };
        checkers blackPoints { {24, 1} };
        auto whiteBar = 0;
        auto blackBar = 0;
        auto whiteOff = 15;
        auto blackOff = 14;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto onRoll = PlayerColor::WHITE;
        auto game = BackgammonTestHelper();

        // Act
        game.setBoard(board);

        // Assert
        REQUIRE(game.isFinished(onRoll));
    }
}

TEST_CASE("Backgammon game result") {

    SECTION("If game hasn't finished yet result is unknown") {
        // Arrange
        checkers whitePoints { };
        checkers blackPoints { {24, 1} };
        auto whiteBar = 1;
        auto blackBar = 0;
        auto whiteOff = 14;
        auto blackOff = 14;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto game = BackgammonTestHelper();

        // Act
        game.setBoard(board);
        auto isFinished = game.isFinished(PlayerColor::WHITE) || game.isFinished(PlayerColor::BLACK);

        // Assert
        REQUIRE_FALSE(isFinished);
        REQUIRE_FALSE(game.getResult());
    }

    SECTION("If BLACK has won and WHITE has some of his checkers off then BLACK wins 1 point") {
        // Arrange
        checkers whitePoints { { 2, 2 } };
        checkers blackPoints {};
        auto whiteBar = 0;
        auto blackBar = 0;
        auto whiteOff = 13;
        auto blackOff = 15;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto game = BackgammonTestHelper();
        auto expectedResult = GameResult(PlayerColor::BLACK, 1);

        // Act
        game.setBoard(board);
        auto isFinished = game.isFinished(PlayerColor::BLACK);
        auto actualResult = game.getResult();

        // Assert
        REQUIRE(isFinished);
        REQUIRE(actualResult);
        REQUIRE(expectedResult.winner == actualResult.value().winner);
        REQUIRE(expectedResult.points == actualResult.value().points);
    }

    SECTION("If WHITE has won with gammon he should get 2 points") {
        // Arrange
        checkers whitePoints {};
        checkers blackPoints { {24, 15} };
        auto whiteBar = 0;
        auto blackBar = 0;
        auto whiteOff = 15;
        auto blackOff = 0;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto game = BackgammonTestHelper();
        auto expectedResult = GameResult(PlayerColor::WHITE, 2);

        // Act
        game.setBoard(board);
        auto isFinished = game.isFinished(PlayerColor::WHITE);
        auto actualResult = game.getResult();

        // Assert
        REQUIRE(isFinished);
        REQUIRE(actualResult);
        REQUIRE(expectedResult.winner == actualResult.value().winner);
        REQUIRE(expectedResult.points == actualResult.value().points);
    }

    SECTION("If WHITE has won with backgammon he should get 2 points") {
        // Arrange
        checkers whitePoints {};
        checkers blackPoints { {6, 1}, {24, 14} };
        auto whiteBar = 0;
        auto blackBar = 0;
        auto whiteOff = 15;
        auto blackOff = 0;
        auto board = BoardState(whitePoints, blackPoints, whiteBar, blackBar, whiteOff, blackOff);
        auto game = BackgammonTestHelper();
        auto expectedResult = GameResult(PlayerColor::WHITE, 3);

        // Act
        game.setBoard(board);
        auto isFinished = game.isFinished(PlayerColor::WHITE);
        auto actualResult = game.getResult();

        // Assert
        REQUIRE(isFinished);
        REQUIRE(actualResult);
        REQUIRE(expectedResult.winner == actualResult.value().winner);
        REQUIRE(expectedResult.points == actualResult.value().points);
    }
}

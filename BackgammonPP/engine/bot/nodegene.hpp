#pragma once

class NodeGene{
    enum nodeType {
        INPUT = 0,
        OUTPUT = 1,
        HIDDEN = 2,
        BIAS = 3
    };

    int number;
    nodeType type;

};

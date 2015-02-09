//
//  rnTestCase.h
//  instancedModeler_rev02
//
//  Created by MatobaHiroshi on 2/9/15.
//
//

#pragma once

class rnTestCase{
    
public:
    
    rnTestCase(){};
    
    void setup_mergin_test();

    void loadRandomSphere(int num, float pos_max);

    void loadRandomCylinder(int num, float pos_max);
};
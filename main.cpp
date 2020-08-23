#include <iostream>

#include "vmfelements.h"
#include "vmfimporter.h"

#include <assert.h>
#include <cstdio>

using namespace std;



int main()
{
    try{
        VMF::VMFImporter importer;
        VMF::Scene* scene = importer.LoadFile("c:/users/tapir/desktop/asd.vmf").scene;

    }catch(std::exception& e){
        std::cout << e.what() << "\n";

    }


    return 0;
}

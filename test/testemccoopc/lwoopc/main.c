#include<stdio.h>
#include"lw_oopc.h"
INTERFACE(IMoveable){
    void (*move)(IMoveable *t);
};
ABS_CLASS(Animal){
    int age;
    void (*setAge)(Animal *t, int age);
    void (*eat)(Animal *t);
    void (*init)(Animal *t, int age);
};
CLASS(Fish){
    EXTENDS(Animal);
    IMPLEMENTS(IMoveable);
    void (*init)(Fish *t, int age);
};

void Animal_setAge(Animal *t, int age){
    t->age = age;
}
void Animal_init(Animal *t, int age){
    t->setAge(t, age);
}
ABS_CTOR(Animal);
FUNCTION_SETTING(setAge, Animal_setAge);
FUNCTION_SETTING(init, Animal_init);
END_ABS_CTOR

void Fish_eat(Animal *t){
    printf("fish eat!\n");
}
void Fish_move(IMoveable *t){
    printf("fish move\n");
}
void Fish_init(Fish *t, int age){
    Animal *animal = SUPER_PTR(t, Animal);
    animal->setAge(animal, age);
}

CTOR(Fish)
SUPER_CTOR(Animal);
FUNCTION_SETTING(Animal.eat,Fish_eat);
FUNCTION_SETTING(IMoveable.move, Fish_move);
FUNCTION_SETTING(init, Fish_init);
END_CTOR

int main(){
    Fish *fish = Fish_new();
    Fish *fish2 = Fish_new();
    Animal *animals[2]={0};
    IMoveable *moveObjs[2]={0};
    int i = 0;
    int j=0;
    fish->init(fish, 1);
    animals[0]=SUPER_PTR(fish, Animal);
    fish2->init(fish2, 3);
    animals[1]=SUPER_PTR(fish2, Animal);
    moveObjs[0] = SUPER_PTR(fish, IMoveable);
    moveObjs[1] = SUPER_PTR(fish2, IMoveable);
    for(i=0;i<2; ++i){
        Animal *animal = animals[i];
        animal->eat(animal);
        IMoveable *moveObj = moveObjs[i];
        moveObj->move(moveObj);
    }
lw_oopc_delete(fish);
lw_oopc_delete(fish2);

    return 0;
}
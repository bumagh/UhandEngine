#include <stdio.h>
#include <unistd.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "obj.h"
classdef(Dog);
class(Dog,
public(
    const char *sound;
    void (*say)();
));
classdef(LoudDog);
class(LoudDog,
public(
    extend(Dog);
));
method(Dog, void, say)(){
    obj_prepare(Dog);
    printf("I say %s\n", self->sound);
}
method(LoudDog, void , say)(){
    obj_prepare(LoudDog);
    printf("%s..%s\n",self->Dog.sound,self->Dog.sound);
}

ctor(Dog)(const char *sound){
    obj_setup(Dog);
    obj_bind(Dog, say);
    self->sound = sound;
    obj_done(Dog);
}
ctor(LoudDog)(const char *sound){
    obj_setup(LoudDog);
    obj_override(LoudDog, Dog, say);
    self->Dog.sound = sound;
    obj_done(LoudDog);
}

int gQuit = 0;

void tick()
{
  
}
void loop()
{
  
#if __EMSCRIPTEN__
        emscripten_set_main_loop(tick, 0, 0);
#else
  while (!gQuit)
    {
        tick();
     
    }
    quit();

#endif
    }
#ifndef __EMSCRIPTEN__
#undef main
#endif
int main(int argc, char *argv[])
{
printf("It's %d bit system \n", sizeof(void *) * 8); 
    //  Dog dog = new(Dog)("woof");
    // LoudDog lddog = new(LoudDog)("ruff");
    // dog->say();
    // lddog->Dog.say();
    // dog->base.release();
    // lddog->base.release();
    loop();
    return 0;
}

# Protoflight
## Introduction

WARNING- this software is a prototype of a prototype. It is not intended for use, nor is it even fully running yet.

This software is an example of a flight software like architecture, containing many of the elements and decisions
that other flight software have such as cFS (Core Flight System) while aiming to be much (much) smaller and
easier to understand.


This is really an experiment in trying to develop in the style of flight software systems for hobby electronics.
I want to get this running on some board, perhaps an STM32F4Discovery, and develop as small and simple a software
system as I can while keeping the general architecture of the flight software systems I have worked with.


## Motivation

The underlying goal here is to explore flight software architecture while
keeping the software very small, so it can be understood and modified for
application-specific needs. Ideally this could become a teaching tool for
others, but at the very least I hope for it to be a learning experience for
myself.


Not all flight software looks like this- protoflight is leaning towards the
"flight computer" niche, where you need a lot of functionality to schedule,
monitor, collect, run complex algorithms, etc. Some flight software systems do
not need all of this functionality and can therefore be simpler and more
direct.

I've develop with cFS in the past, and found that while it covers most needs of
a flight software systems, and it delivers on the promise of modularity and
portability, I was weighed down by its size on small systems (not in
performance, but mostly in my own time and ability to modify software of that
size).

## Goals

  * Small size - this software should not accumulate features, and should remain in the few thousands of lines range. Size
    by itself is not the only goal, as some features are worth extra code, but should be taken into account.
  * Portability - this software should not rely on its operating system or board directly. Project-specific modules
    are welcome to depend on anything they like, but the core software system should not. At the very least it must
    run on Linux on my laptop and FreeRTOS on at least one board.
  * Consistency - the software should be consistent in every definition, comment, and line of code.
  * Testability - ideally the software should be designed in a way that allows testing. For example: most logic, especially
    conditional statements, loops, etc, should be in functions that do not use the operating system or rely on any
    external interface to assist in simple and repeatable unit testing.
  * Extensibility - the software should be extendable with additional software modules without too much effort when designing
    and implementing a new module.
  * Best Practice - this software should show good practices. It does not have to be perfect, but in general it should track
  metrics, cover error cases, report information to the user, use good architecture, etc, to the extent that I am able to accomplish.

## The Name Protoflight

The name protoflight comes from a phrase used to describe the situation where, instead of building an engineering unit and then
a flight unit, a  project decides to build just a flight unit to reduce cost and schedule while increasing risk. The name combines
'prototype' and 'flight' indicating that the prototype is the unit that the project will fly- a protoflight.

I chose this name because it is an aerospace term, which is appropriate for an exploration of flight software design, and to indicate
that using this software increases your risk as it is not really intended for production use.

## TODO

Modules:
    * tbl does not yet load any tables.
    * there is no fault monitoring.
    * tm does not do task monitoring- this should be part of tm\_running and incorporate some kind of watchdog.
    * command scheduling
    * relative time sequences
    * absolute time sequences

Testing:
    * test coverage is not yet collected.
    * tm modules does not yet have unit tests.

Abstraction:
    * the tinypthreads build should be tested or removed.
    * there should be at least one more os layer, ideally FreeRTOS.

Static Analysis:
    * some level of static analysis should be run- cppcheck and/or cobra for example.
    * the compiler flags could be much stricter.

Ground System:
    * ideally there would be a ground system to work with, likely Ball COSMOS.

Best Practice:
    * add documentation generation with pandoc and markdown files as an example.
    * run with valgrind for memory and threading.


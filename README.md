# Protoflight
## Introduction
WARNING- this software is a prototype of a prototype. It does not yet even compile.

This software is an example of a flight software like architecture, containing many of the elements and decisions
that other flight software have, such as cFS (Core Flight System), while aiming to be much (much) smaller and
easier to understand.


This is really an experiment in trying to develop in the style of flight software systems for hobby electronics.
I want to get this running on some board, perhaps an STM32F4Discovery, and develop as small and simple a software
system as I can.


## Motivation
The underlying goal here is to explore flight software architecture while keeping the software very small, so it can
be understood and modified for application-specific needs. Ideally this could become a teaching tool for others,
but at the very least I hope for it to be a learning experience for myself.

I've develop with cFS in the past, and found that while it
covers most needs of a flight software systems, and it delivers on the promise of modularity and portability, I was
weighed down by its size on small systems (not in performance, but mostly in my own time and ability to modify software
of that size).

## Goals
  * Small size - this software should not accumulate features, and should remain in the few thousands of lines. Size
    by itself is not the only goal, as some features are worth extra code, but should be taken into account.
  * Portability - this software should not rely on its operating system or board directly. Project-specific modules
    are welcome to depend on anything they like, but the core software system should not.
  * Consistency - the ensure software system should ensure that every definition, comment, and line of code
    comply with a consistent style.
  * Testability - ideally the software should be designed in a way that allows testing. For example: most logic, especially
    conditional statements, loops, etc, should be in functions that do not use the operating system or rely on any
    external interface to assist in simple and repeatable unit testing.
  * Extensibility - the software should be extendable with additional software modules without too much effort when designing
    and implementing a new module.

## The Name Protoflight
The name protoflight comes from a phase used to describe the situation where, instead of building an engineering unit and then
a flight unit, a  project decides to build just a flight unit to reduce cost and schedule while increasing risk. The name combines
'prototype' and 'flight' indicating that the prototype is the unit that the project will fly.

I chose this name because it is an aerospace term, which is appropriate for an exploration of flight software design, and to indicate
that using this software increases your risk as it is not really intended for production use.


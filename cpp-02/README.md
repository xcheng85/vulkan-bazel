## concept
generic programming
template programming

function template = generic function
universal reference = forward reference



## compilation issue
template without caller does not generate code in binary ? 
workaround: generate object files with exlicit form

call template = instantiation


## Perfect forwarding: For conditional casting
1. univeral reference

## ADT: interface, virtual class

## class vs struct ? 
class: abstraction
struct: data-centric helper

## modern constructor with initialization list

## when default copy constructor not work ? 
class has pointer. doulbe pointer

## explict attribute for construct ? 
avoid implicit conversion

## can constructor call other constructor
delegate constructor

## uniform initialization

## designated initialization

## how to turn an lvalue to rvalue
std::move()

## how to pass not change lvalue/rvalue
std::forward()

## compiler's copy elision makes copy/move ctor less usable 
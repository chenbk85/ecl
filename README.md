ecl
===

Very small C++ class set suitable for embedded devices.
C++11-compatible compiler needed. (Tested: gcc>=4.7, clang>=3.5)

### fsm
Generic class for Finite State Machine creating.
Influenced by boost::MSM, but more simple.

### bit_field
Class for serialization and deserialization of bitfield-like structures.
Main problem in bitfields is compiler-specific implementation and ending problems.
bitfield class architecture independent. Of course, at cost of complexity.

### scope_guard
RAII idiom. Class provides mechanism to define execution of some code on scope exit.

### command processor & command
Simple classes suitable for composing command processors on embedded systems.
Commands can be arranged as tree.

### json
JSON serializer/deserializer.
JSON document specialized as template, than it can be serialized to ecl::stream,
std::stream or any other object that support operator<< and deserialized from C-style string (char *)

### stream
Simple implementation of stream. (iostream is too complex for embedded systems and uses dynamic memory)
Flush function can be specialized as template arg. It will be called, when stream becomes full.

### singleton
Meyers singleton implementation.

### str_const
Scott Schurr's str_const class.

### circular_buffer
Circular buffer class.

### tree
Binary tree implementation. Can be used with statically allocated nodes, so no memory allocation needed.

### red_black_tree
Red-black tree implementation. Can be used with statically allocated nodes, so no memory allocation needed.

### static_map
Simple map class with constant fields. Uses RB-tree for element access.

### web server (under development)
Simple web server with staticaly linked resources.
Simple CGI support.

No gcc-arm-none-eabi demo. For such demo we need FreeRTOS, for example, and lwIP.

build
===

Targets:

    * all - all tests
    * test_fsm
    * test_scope_guard
    * test_circular_buffer
    * test_bitfield
    * test_singleton
    * test_stream
    * test_json
    * test_str_const
    * test_tree
    * test_static_map
    * test_command_processor
    * test_web

gcc:

    make all

clang:

    CXX=clang++ make all

arm-none-eabi-gcc:

    CXX=arm-none-eabi-g++ ADD_FLAGS=--specs=rdimon.specs make tests_without_web

tests
===

All tests can be compiled by gcc, clang, gcc-arm-none-eabi. Except web_server(no arm-none-eabi)
Web server resources generated by script res_gen.sh

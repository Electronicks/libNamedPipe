
cmake_minimum_required(VERSION 2.8)
Project(libNamedPipe)


file(GLOB NamedPipe_src PosixNamedPipe.cpp WinNamedPipe.cpp NamedPipeServer.cpp )
file(GLOB NamedPipe_h PosixNamedPipe.h INamedPipe.h NamedPipe.h NamedPipeServer.h WinNamedPipe.h)

add_library(NamedPipe STATIC ${NamedPipe_src} ${NamedPipe_h})
SET_TARGET_PROPERTIES(NamedPipe PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (NamedPipe)

file(GLOB NamedPipeTestClient_src ./examples/NamedPipeTestClient.cpp )
file(GLOB NamedPipeTestClient_h PosixNamedPipe.h INamedPipe.h NamedPipe.h)

add_executable(NamedPipeTestClient ${NamedPipeTestClient_src} ${NamedPipeTestClient_h})
SET_TARGET_PROPERTIES(NamedPipeTestClient PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (NamedPipeTestClient NamedPipe)

file(GLOB NamedPipeTestServer_src ./examples/NamedPipeTestServer.cpp ./examples/SimpleServer.cpp )
file(GLOB NamedPipeTestServer_h ./examples/SimpleServer.h)

add_executable(NamedPipeTestServer ${NamedPipeTestServer_src} ${NamedPipeTestServer_h})
SET_TARGET_PROPERTIES(NamedPipeTestServer PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (NamedPipeTestServer NamedPipe)





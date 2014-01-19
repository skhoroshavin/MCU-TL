project(AVRTest)
cmake_minimum_required(VERSION 2.8)
add_definitions(-Os -std=c++11)

include_directories(mcutl mcutl/AVR)

set( SRC_LIST
	mcutl/AVR/GPIO.h
	mcutl/AVR/Timer.h
	mcutl/AVR/Usart.h
	mcutl/BaseGPIO.h
	mcutl/BaseStream.h
	mcutl/Dispatcher.h
	mcutl/Flash.h
	mcutl/Interrupts.h
	mcutl/MetaList.h
	mcutl/MetaUtils.h
	mcutl/PinList.h
	mcutl/Registers.h
	mcutl/RingBuffer.h
	main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})


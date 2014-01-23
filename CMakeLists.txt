cmake_minimum_required(VERSION 2.8)

project(MCUTL)
add_definitions(-Os -std=c++11)

include_directories(.)

set( MCUTL
	upload.sh

	meta/common.h
	meta/list.h

	mcutl/fsm.h
	mcutl/ring_buffer.h

	mcutl/flash.h
	mcutl/interrupts.h
	mcutl/registers.h
	mcutl/ports.h
	mcutl/timers.h
	mcutl/usart.h

	mcutl/dispatcher.h
	mcutl/pin_list.h

	mcutl/avr/_ports.h
	mcutl/avr/_timers.h
	mcutl/avr/_usart.h
)

add_custom_target(${PROJECT_NAME} SOURCES ${MCUTL})

add_subdirectory(examples)

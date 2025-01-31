# Habilitar o SDK do Raspberry Pi Pico
cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

# Nome do projeto
project(interLed C CXX ASM)

# Iniciar o SDK Pico
pico_sdk_init()

# Adicionar o executável principal
add_executable(interLed
    interLed.c              # O arquivo principal
)

# Gerar o cabeçalho do código PIO
pico_generate_pio_header(interLed
    ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio
)

# Linkar as bibliotecas necessárias
target_link_libraries(interLed
    pico_stdlib
    hardware_pio
    hardware_clocks
    pico_binary_info
    hardware_pwm
)

# Habilitar saída USB (opcional)
pico_enable_stdio_usb(interLed 1)
pico_enable_stdio_uart(interLed 0)

# Criação do executável
pico_add_extra_outputs(interLed)





set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Add executable. Default name is the project name, version 0.1

pico_set_program_name(interLed "interLed")
pico_set_program_version(interLed "0.1")


# Add the standard include files to the build
target_include_directories(interLed PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(interLed 
        
        )

pico_add_extra_outputs(interLed)


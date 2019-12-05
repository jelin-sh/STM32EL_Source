#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

# specify cross compilers and tools
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32EL/STM32EL_Kernal/System/STM32F427IIHx_FLASH.ld)

#Uncomment for hardware floating point
SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING)

SET(COMMON_FLAGS
    "-mcpu=cortex-m4 ${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections \
    -g -fno-common -fmessage-length=0 -specs=nosys.specs -specs=nano.specs")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections,--print-memory-usage -u _printf_float -T  ${LINKER_SCRIPT}")

PROJECT(STM32EL_STM32F427IIH6 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)

add_definitions(-D__weak=__attribute__\(\(weak\)\) -D__packed=__attribute__\(\(__packed__\)\) -DUSE_HAL_DRIVER -DSTM32F427xx)

add_definitions(-DFreeRTOS_TOTAL_HEAP_SIZE=50*1024)
add_definitions(-DMain_TOTAL_HEAP_SIZE=512)
file(GLOB_RECURSE SOURCES "Middlewares/*.*"  "STM32EL/*.*" "User/*.*")

include_directories(
        STM32EL/STM32EL_BaseLibrary/HAL_STM32F4/Inc/cmsis
        STM32EL/STM32EL_BaseLibrary/HAL_STM32F4/Inc/driver
        STM32EL/STM32EL_BaseLibrary/HAL_STM32F4/Inc/driver/Legacy
        STM32EL/STM32EL_BaseLibrary/FreeRTOS
        STM32EL/STM32EL_BaseLibrary/FreeRTOS/include
        STM32EL/STM32EL_BaseLibrary/FreeRTOS/portable
        STM32EL/STM32EL_BaseLibrary/FreeRTOS/portable/GCC/ARM_CM4F
        STM32EL/STM32EL_BaseLibrary/FreeRTOS/portable/MemMang
        STM32EL/STM32EL_BaseLibrary/FreeRTOS/CMSIS_RTOS

        STM32EL/STM32EL_Kernal/System
        STM32EL/STM32EL_Kernal/RCC
        STM32EL/STM32EL_Kernal/Thread
        STM32EL/STM32EL_Kernal/IOPort
        STM32EL/STM32EL_Kernal/SerialPort
        STM32EL/STM32EL_Kernal/OutPutStream
        STM32EL/STM32EL_Kernal/Tool
        STM32EL/STM32EL_Kernal/InPutStream
        STM32EL/STM32EL_Kernal/WatchDog
        STM32EL/STM32EL_Kernal/Timer
        STM32EL/STM32EL_Kernal/PWM
        STM32EL/STM32EL_Kernal/SPI
        STM32EL/STM32EL_Kernal/CAN
        STM32EL/STM32EL_Kernal/DMA

        User
        User/RoboMasterPackge/MPU6500
)


add_executable(${PROJECT_NAME} ${SOURCES} ${LINKER_SCRIPT})


set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map")

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

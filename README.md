# eBBTF Objective # (Embedded Board Bring Up Test Framework)

=================================================================

eBBTF is a test suite specifically designed for embedded board bring up
testing.

Board bring-up test consists of tests for the devices and functional sections 
of the electronic board that would make up the embedded product. These tests 
shall be used to validate the hardware board during its development, typically 
in multiple iterations in order to ensure the readiness for the high volume 
manufacturing. A board diagnostics software shall be developed specific to the 
platform based on the requirements from electrical team to test all interfaces,
peripherals, etc. In a manufacturing test configuration, the board bring-up tests 
can be reused along with additional tests to test the large quantity of hardware in 
parallel. These tests are typically run continuously for regression testing to identify 
manufacturing issues.

The approaches that gets employed during these testing’s has several limitations 
such as developing a board diagnostics software each time for a new hardware, 
using legacy fixtures that are very expensive & time consuming, inconsistency in 
test results, missing out the needs for different test use case, etc.

eBBTF provides a test framework with prepacked test suite to test different interfaces,
devices during board bring up and manufacturing configuration with support for multiple
platforms covering different device technologies (e.g memory,sensors,networking etc.)
eBBTF goal is to distribute framework,testsuite and allows individuals and organizations 
to easily run the available test cases , create and run their own tests,and at the same 
time allows people to share their tests and test results with each other.

Below are some of the key objectives and milestones listed

* Support for multiple platforms
* Covering several device test cases of different technologies 
  (e.g memory,sensors,networking etc.)
* User Interface for test execution
* Test cases reusability across platforms
* Easy deployement using minimal customizations/Modifications
* Detailed failure analysis

# eBBTF Overview (Embedded Board Bring Up Test Framework) #

================================================================

eBBTF is a test framework to validate a hardware board during its 
development typically in multiple iterations in order to ensure the 
readiness for the high volume manufacturing. 

It is based on Das U-Boot Bootloader.

Das U-Boot is an open-source, primary boot loader used in embedded devices 
to package the instructions to boot the device's operating system kernel. 
It is available for a number of computer architectures, including 68k, ARM, 
Blackfin, MicroBlaze, MIPS, Nios, SuperH, PPC, RISC-V and x86.The development 
of U-Boot is closely related to Linux: some parts of the source code originate 
in the Linux source tree, we have some header files in common, and special 
provision has been made to support booting of Linux images.

eBBTF uses U-Boot's infrastructure, such as

* Enviromental variables
* U-Boot Shell
* Linker List
* U-Boot Command Macros
* Kbuild
* U-Boot Board Configuration options

eBBTF provides

* Several device drivers test cases like RTC,DMA,AUDIO,GPIO,ETHERNET,FLASH memories,eMMC
* Support for different SoCs and chipsets
* eBBTF framework and testsuite resides in eBBTF/test-framework/.
* eBBTF works like any other U-Boot command with options

NOTE : eBBTF supports only hardware platforms which is supported by u-boot.

# eBBTF Features#

================================================================

* Easy to use(filter test cases not applicable for platform)
* Easy to support new platforms
* High Code Reuse across platforms
* Prepacked Test suites
* Compatible with any version of Das U-boot

# Test Suites Details #

================================================================

The following device test cases for a specified part numbers are available . 
SoC represents the test for system on Chip interfaces like SPI,I2C,CPU cores etc.  

Driver Category | Device | Part Number | Test Case |
| ------------- | -------| ----------  | --------- |
| Memory        | EEPROM | AT243C2 | Verifies Data Integrity by writing and reading to the EEPROM | 
|               | NOR FLASH | W25Q32 | 1. Performs Erase,Write and Read |
|               |           |        | 2. Verifies Data Integrity |
| 		| eMMC   | eMMC | 1. Verfies eMMC detection |
|               |        |      | 2. Performs Erase,Write and Read Operations |
|               | System Memory | DDR | 1. Tests Data Bus |
|               |               |     | 2. Tests Address Bus |
|               |               |     | 3. Performs Read/Write Pattern Test |
| Timer         | RTC | DS2321 | 1.Set Time |
|              	|     |	       | 2.Get Time |
| Network       | Ethernet | SoC | 1.Interface Link |
|          	|          |     | 2.IP Address Connectivity using PING|
|          	|          |     | 3.Speed & duplex test |
|          	|          |     | 4.Auto negotiate test |
| Power Mgmt    | Power Rail | TPS65217 | Reads Power mgmt related registers and displays on console | 
| Input/Output  | GPIO | SoC | 1.Test GPIO Output |
|      		|      |     | 2.Test GPIO Input |
| Serial        | I2C | SoC | Scans I2C Bus for connected slaves |

# Platform Under Tests Supported #

=================================================================

eBBTF has been used on following platforms and evaluation Boards:

* Beagle Bone Black EVM (AM35XX)
* Raspberry Pi 2 EVM (BCM2835)

# Host Requirements #

=================================================================

* Linux Host (Any distributions) 
* GCC Toolchain for ARM based platforms
* Serial console terminal application
* GIT

# Target Device Requirements #

=================================================================

* U-boot used for the corresponding platform shall be functional
* U-Boot Programmer
* Serial console

# Build & Installation #

================================================================

Clone the project at
	https://github.com/arjunTest3330/eBBTF.git

Detailed Build & Installation instructions are in the Getting-Started file
	
# Running Tests #

================================================================

eBBTF can be executed like any other U-Boot Command functions. 
A command named "boarddiag" can be used to start the test

boarddiag - Board diagnostics Software

	Usage:
	boarddiag [args...]
   		list           -- List available tests
   		run <test_num> -- Run individual test by test ID specified in the list
   		runAll         -- Run all tests
          		loop <times> -- Set loop to execute a test n number of times. 

The framework  assumes a serial console is available for executing the 
test cases

# LICENSE #

=================================================================

The test suite follows Das U-boot Licensing (GPL2 or later).

Refer LICENSE for further details

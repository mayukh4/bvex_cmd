# BVEX Ground Station Control System

A command-line interface for controlling the Balloon-borne VLBI Experiment (BVEX) ground station system, developed in collaboration with **Felix Thiel** and **Terry Yang**.

## Overview

This system provides ground control for two primary computers:
- **Ophiuchus**: Telescope control and camera operations
- **Sagittarius**: Receiver, RFSoC, VLBI, and timing operations

The system uses UDP socket communication to send commands and receive acknowledgments from both computers.

## Architecture

The ground station operates using a packet-based protocol:
- Commands are translated to enumerated values and packaged into structured packets
- Packets include checksums for integrity verification
- Each command receives acknowledgment with command counters
- Supports targeting individual computers (Oph/Sag) or both simultaneously

## Files

- `main_ground.c` - Main entry point and initialization
- `cli_ground.c` - Core command processing and network communication
- `cli_ground.h` - Header file with data structures and function declarations
- `bcp_ground.config` - Configuration file with network settings

## Compilation

Requires `libconfig` for configuration file parsing:

```bash
# Install libconfig (Ubuntu/Debian)
sudo apt-get install libconfig-dev

# Compile
gcc -o bcp_ground main_ground.c cli_ground.c -lconfig -lm
```

## Configuration

Edit `bcp_ground.config`:

```
main:
{
  cmd_log = "/path/to/log/directory";
  sag_ip = "IP_ADDRESS_SAGITTARIUS";
  oph_ip = "IP_ADDRESS_OPHIUCHUS";  
  sag_port = 8090;
  oph_port = 8004;
  ls_enabled = 0;    # Limit switch enabled (0/1)
  timeout = 1;       # Socket timeout in seconds
};
```

## Usage

```bash
./bcp_ground <config_file> <hostname>
```

Example:
```bash
./bcp_ground bcp_ground.config ground_station_1
```

## Command Categories

### System Commands
- `test_Oph` / `test_Sag` - Test connections
- `reset_counter_Oph` / `reset_counter_Sag` - Reset command counters
- `exit` - Exit ground station
- `exit_bcp` - Exit BCP on both computers
- `help` - Show command list

### Telescope Commands (Ophiuchus)
- `lock_tel_cmd` / `unlock_tel_cmd` - Lock/unlock telescope
- `motor_start` / `motor_stop` - Motor control
- `gotoenc <az>,<el>` - Go to position (degrees)
- `enctrack <ra>,<dec>` - Track celestial coordinates
- `encdither <start_el>,<stop_el>,<scan_vel>,<nscans>` - Elevation dither scan
- `park` - Park telescope

### Camera Commands (Ophiuchus) 
- `bvexcam_start` / `bvexcam_stop` - Camera control
- `bvexcam_set_exp <time_ms>` - Set exposure (9.6-1000ms)
- `bvexcam_solve_start` / `bvexcam_solve_stop` - Astrometry solving
- `focus_bvexcam` - Auto focus

### Receiver Commands (Sagittarius)
- `start_spec` / `stop_spec` - Spectrometer control
- `start_gps` / `stop_gps` - GPS control
- `receiver_start` / `receiver_stop` - Receiver control

### RFSoC Commands (Sagittarius)
- `rfsoc_on` / `rfsoc_off` - RFSoC power (includes 40s boot timer)
- `rfsoc_configure_ocxo` - Configure OCXO

### VLBI Commands (Sagittarius)
- `start_vlbi <ssd_id>` - Start VLBI recording (ssd_id: 1 or 2)
- `stop_vlbi` - Stop VLBI recording
- `start_backend` / `stop_backend` - Backend control (includes 60s timer)

### Timing Commands (Sagittarius)
- `start_timing_chain` / `stop_timing_chain` - Timing chain control
- `start_ticc` / `stop_ticc` - TICC control

## Safety Features

- Command confirmation prompts for critical operations
- Parameter validation with range checking
- Connection status monitoring
- Command logging with timestamps
- Automatic boot/startup timers for RFSoC and backend systems

## Network Protocol

Commands are sent as formatted UDP packets containing:
- Start/end bytes for packet framing
- Command enumeration and timestamp
- Integer and floating-point parameter arrays
- XOR checksum for integrity verification
- Destination targeting (Oph/Sag/Both)

## Logging

All commands are logged to timestamped files in the configured log directory:
```
/path/to/logs/cmd_logs_<timestamp>.txt
```

## Error Handling

- Socket timeouts for unresponsive computers
- Command retry logic for failed transmissions
- Checksum verification for packet integrity
- Parameter range validation before transmission

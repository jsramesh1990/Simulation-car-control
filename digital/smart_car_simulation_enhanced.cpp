#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
    #define USE_WINDOWS 1
#else
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
    #define USE_WINDOWS 0
#endif

using namespace std;

// Color codes for terminal output
namespace Color {
    const string RED = "\033[0;31m";
    const string GREEN = "\033[0;32m";
    const string YELLOW = "\033[1;33m";
    const string BLUE = "\033[0;34m";
    const string CYAN = "\033[0;36m";
    const string MAGENTA = "\033[0;35m";
    const string WHITE = "\033[1;37m";
    const string BOLD = "\033[1m";
    const string RESET = "\033[0m";
    const string BG_RED = "\033[41m";
    const string BG_GREEN = "\033[42m";
    const string BG_YELLOW = "\033[43m";
    const string BG_BLUE = "\033[44m";
}

// Log entry structure
struct LogEntry {
    string timestamp;
    string message;
    
    LogEntry(const string& msg) {
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);
        char buffer[10];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
        timestamp = string(buffer);
        message = msg;
    }
    
    string toString() const {
        return "[" + timestamp + "] " + message;
    }
};

// Graphics class for terminal rendering
class TerminalGraphics {
private:
    int terminalWidth;
    int terminalHeight;
    
public:
    TerminalGraphics() {
        terminalWidth = 80;
        terminalHeight = 24;
    }
    
    // Draw horizontal bar graph
    void drawBar(const string& label, int value, int maxValue, int width, const string& color) {
        int barLength = (value * width) / maxValue;
        cout << label << ": [";
        for (int i = 0; i < width; i++) {
            if (i < barLength) {
                cout << color << "█" << Color::RESET;
            } else {
                cout << "░";
            }
        }
        cout << "] " << value << "%" << endl;
    }
    
    // Draw circular speedometer
    void drawSpeedometer(int speed) {
        cout << Color::CYAN << Color::BOLD << "┌─────────────────────────────────────────┐" << Color::RESET << endl;
        cout << Color::CYAN << Color::BOLD << "│           DIGITAL SPEEDOMETER           │" << Color::RESET << endl;
        cout << Color::CYAN << Color::BOLD << "├─────────────────────────────────────────┤" << Color::RESET << endl;
        
        // Large digital speed display
        string speedStr = to_string(speed);
        while (speedStr.length() < 3) speedStr = " " + speedStr;
        
        cout << "│";
        if (speed < 40) cout << Color::GREEN;
        else if (speed < 100) cout << Color::YELLOW;
        else cout << Color::RED;
        
        cout << "              ╔═════════════════╗               " << Color::RESET;
        cout << Color::CYAN << "│" << endl;
        
        cout << "│";
        if (speed < 40) cout << Color::GREEN;
        else if (speed < 100) cout << Color::YELLOW;
        else cout << Color::RED;
        
        cout << "              ║    " << speedStr << " km/h    ║               " << Color::RESET;
        cout << Color::CYAN << "│" << endl;
        
        cout << "│";
        if (speed < 40) cout << Color::GREEN;
        else if (speed < 100) cout << Color::YELLOW;
        else cout << Color::RED;
        
        cout << "              ╚═════════════════╝               " << Color::RESET;
        cout << Color::CYAN << "│" << endl;
        
        // Analog needle representation
        cout << "│";
        int needlePos = (speed * 35) / 180;
        cout << "              ";
        for (int i = 0; i < 35; i++) {
            if (i == needlePos) {
                cout << Color::RED << "▲" << Color::RESET;
            } else if (i % 5 == 0) {
                cout << Color::CYAN << "│" << Color::RESET;
            } else {
                cout << "─";
            }
        }
        cout << "              " << Color::CYAN << "│" << endl;
        
        cout << "│";
        cout << "              0";
        for (int i = 0; i < 7; i++) cout << " ";
        cout << "60";
        for (int i = 0; i < 7; i++) cout << " ";
        cout << "120";
        for (int i = 0; i < 6; i++) cout << " ";
        cout << "180";
        cout << "              " << Color::CYAN << "│" << endl;
        
        cout << Color::CYAN << Color::BOLD << "└─────────────────────────────────────────┘" << Color::RESET << endl;
    }
    
    // Draw fuel gauge
    void drawFuelGauge(int fuel) {
        cout << Color::YELLOW << Color::BOLD << "┌─────────────────────────────────────────┐" << Color::RESET << endl;
        cout << Color::YELLOW << Color::BOLD << "│              FUEL GAUGE                 │" << Color::RESET << endl;
        cout << Color::YELLOW << Color::BOLD << "├─────────────────────────────────────────┤" << Color::RESET << endl;
        
        // Fuel tank icon
        cout << "│  ╔══════════════════════════════════════╗  │" << endl;
        cout << "│  ║";
        
        int fillLevel = (fuel * 38) / 100;
        for (int i = 0; i < 38; i++) {
            if (i < fillLevel) {
                if (fuel > 66) cout << Color::GREEN << "█" << Color::RESET;
                else if (fuel > 33) cout << Color::YELLOW << "█" << Color::RESET;
                else cout << Color::RED << "█" << Color::RESET;
            } else {
                cout << " ";
            }
        }
        cout << "║  │" << endl;
        cout << "│  ╚══════════════════════════════════════╝  │" << endl;
        
        // Fuel percentage with color
        cout << "│";
        if (fuel > 66) cout << Color::GREEN;
        else if (fuel > 33) cout << Color::YELLOW;
        else cout << Color::RED;
        
        cout << "                    " << fuel << "% FULL                     ";
        cout << Color::RESET << Color::YELLOW << "│" << endl;
        
        // Range estimation
        int range = fuel * 12;
        cout << "│                  Range: " << range << " km                    │" << endl;
        
        cout << Color::YELLOW << Color::BOLD << "└─────────────────────────────────────────┘" << Color::RESET << endl;
    }
    
    // Draw tachometer (RPM)
    void drawTachometer(int rpm) {
        cout << Color::MAGENTA << Color::BOLD << "┌─────────────────────────────────────────┐" << Color::RESET << endl;
        cout << Color::MAGENTA << Color::BOLD << "│              TACHOMETER                 │" << Color::RESET << endl;
        cout << Color::MAGENTA << Color::BOLD << "├─────────────────────────────────────────┤" << Color::RESET << endl;
        
        int rpmNeedle = (rpm * 35) / 8000;
        cout << "│  RPM: " << rpm << "                    │" << endl;
        cout << "│  ";
        for (int i = 0; i < 35; i++) {
            if (i == rpmNeedle) {
                cout << Color::RED << "▼" << Color::RESET;
            } else if (i % 7 == 0) {
                cout << Color::MAGENTA << "│" << Color::RESET;
            } else {
                cout << "─";
            }
        }
        cout << "  │" << endl;
        cout << "│  0";
        for (int i = 0; i < 8; i++) cout << " ";
        cout << "2K";
        for (int i = 0; i < 7; i++) cout << " ";
        cout << "4K";
        for (int i = 0; i < 7; i++) cout << " ";
        cout << "6K";
        for (int i = 0; i < 7; i++) cout << " ";
        cout << "8K  │" << endl;
        
        // Redline warning
        if (rpm > 6000) {
            cout << Color::RED << "│  ⚠ WARNING: HIGH RPM! REDLINE APPROACHING! ⚠  │" << Color::RESET << endl;
        }
        
        cout << Color::MAGENTA << Color::BOLD << "└─────────────────────────────────────────┘" << Color::RESET << endl;
    }
    
    // Draw engine status with animation
    void drawEngineStatus(bool engineOn, int throttle) {
        cout << Color::GREEN << Color::BOLD << "┌─────────────────────────────────────────┐" << Color::RESET << endl;
        cout << Color::GREEN << Color::BOLD << "│            ENGINE STATUS                 │" << Color::RESET << endl;
        cout << Color::GREEN << Color::BOLD << "├─────────────────────────────────────────┤" << Color::RESET << endl;
        
        if (engineOn) {
            cout << "│  " << Color::GREEN << "● ENGINE RUNNING" << Color::RESET;
            // Animated throttle bar
            cout << "                    │" << endl;
            cout << "│  Throttle: [";
            int throttleBars = (throttle * 20) / 100;
            for (int i = 0; i < 20; i++) {
                if (i < throttleBars) {
                    cout << Color::GREEN << "█" << Color::RESET;
                } else {
                    cout << "░";
                }
            }
            cout << "] " << throttle << "%  │" << endl;
        } else {
            cout << "│  " << Color::RED << "● ENGINE STOPPED" << Color::RESET;
            cout << "                    │" << endl;
            cout << "│  Throttle: [░░░░░░░░░░░░░░░░░░░░] 0%  │" << endl;
        }
        
        cout << Color::GREEN << Color::BOLD << "└─────────────────────────────────────────┘" << Color::RESET << endl;
    }
    
    // Draw brake system status
    void drawBrakeStatus(int brakePressure) {
        cout << Color::RED << Color::BOLD << "┌─────────────────────────────────────────┐" << Color::RESET << endl;
        cout << Color::RED << Color::BOLD << "│            BRAKE SYSTEM                  │" << Color::RESET << endl;
        cout << Color::RED << Color::BOLD << "├─────────────────────────────────────────┤" << Color::RESET << endl;
        
        cout << "│  Pressure: ";
        if (brakePressure > 66) cout << Color::GREEN;
        else if (brakePressure > 33) cout << Color::YELLOW;
        else cout << Color::RED;
        
        cout << "[";
        int pressureBars = (brakePressure * 25) / 100;
        for (int i = 0; i < 25; i++) {
            if (i < pressureBars) {
                cout << "█";
            } else {
                cout << "░";
            }
        }
        cout << "] " << brakePressure << "%" << Color::RESET << "  │" << endl;
        
        // Brake wear indicator
        int wear = 100 - brakePressure;
        cout << "│  Wear:     ";
        if (wear < 30) cout << Color::GREEN;
        else if (wear < 60) cout << Color::YELLOW;
        else cout << Color::RED;
        
        cout << "[";
        int wearBars = (wear * 25) / 100;
        for (int i = 0; i < 25; i++) {
            if (i < wearBars) {
                cout << "█";
            } else {
                cout << "░";
            }
        }
        cout << "] " << wear << "%" << Color::RESET << "  │" << endl;
        
        if (brakePressure < 30) {
            cout << Color::RED << "│  ⚠ BRAKE PRESSURE LOW! SERVICE REQUIRED! ⚠   │" << Color::RESET << endl;
        }
        
        cout << Color::RED << Color::BOLD << "└─────────────────────────────────────────┘" << Color::RESET << endl;
    }
    
    // Draw warning indicators
    void drawWarnings(const string& warning, bool faultActive, const string& faultCode) {
        cout << Color::YELLOW << Color::BOLD << "┌─────────────────────────────────────────┐" << Color::RESET << endl;
        cout << Color::YELLOW << Color::BOLD << "│            WARNING PANEL                 │" << Color::RESET << endl;
        cout << Color::YELLOW << Color::BOLD << "├─────────────────────────────────────────┤" << Color::RESET << endl;
        
        if (warning != "None") {
            cout << "│  " << Color::RED << "⚠⚠⚠  " << warning << "  ⚠⚠⚠" << Color::RESET;
            int spaces = 27 - warning.length();
            for (int i = 0; i < spaces; i++) cout << " ";
            cout << "│" << endl;
        } else {
            cout << "│  " << Color::GREEN << "✓ All systems normal" << Color::RESET;
            cout << "                      │" << endl;
        }
        
        if (faultActive) {
            cout << "│  " << Color::RED << "🔧 FAULT: " << faultCode << Color::RESET;
            int spaces = 23 - faultCode.length();
            for (int i = 0; i < spaces; i++) cout << " ";
            cout << "│" << endl;
        }
        
        cout << Color::YELLOW << Color::BOLD << "└─────────────────────────────────────────┘" << Color::RESET << endl;
    }
    
    // Draw live animation frame
    void drawLiveFrame(int speed, int fuel, bool engineOn, int throttle, int brakePressure, 
                       const string& warning, bool faultActive, const string& faultCode) {
        int rpm = speed * 30;
        
        cout << "\n";
        cout << Color::CYAN << Color::BOLD << "╔══════════════════════════════════════════════════════════════════╗" << Color::RESET << endl;
        cout << Color::CYAN << Color::BOLD << "║           SMART CAR LIVE DASHBOARD - REAL TIME DISPLAY          ║" << Color::RESET << endl;
        cout << Color::CYAN << Color::BOLD << "╚══════════════════════════════════════════════════════════════════╝" << Color::RESET << endl;
        cout << endl;
        
        // Top row - Speedometer and Fuel Gauge
        drawSpeedometer(speed);
        cout << endl;
        drawFuelGauge(fuel);
        cout << endl;
        
        // Middle row - Tachometer and Engine Status
        drawTachometer(rpm);
        cout << endl;
        drawEngineStatus(engineOn, throttle);
        cout << endl;
        
        // Bottom row - Brake and Warnings
        drawBrakeStatus(brakePressure);
        cout << endl;
        drawWarnings(warning, faultActive, faultCode);
        
        cout << endl;
        cout << Color::CYAN << "═══════════════════════════════════════════════════════════════════" << Color::RESET << endl;
        
        // Live ticker animation
        static int tickerPos = 0;
        const char* tickerText = "► LIVE SIMULATION ACTIVE • REAL-TIME DATA • PRESS MENU OPTIONS ◄";
        cout << "  ";
        for (int i = 0; i < 60; i++) {
            int idx = (tickerPos + i) % strlen(tickerText);
            cout << tickerText[idx];
        }
        tickerPos = (tickerPos + 1) % strlen(tickerText);
        cout << endl;
        cout << Color::CYAN << "═══════════════════════════════════════════════════════════════════" << Color::RESET << endl;
    }
};

// Car System Class
class SmartCarSystem {
private:
    // Core parameters
    int speed;
    int fuel;
    bool engineOn;
    string warning;
    int brakePressure;
    int throttle;
    bool faultActive;
    string faultCode;
    bool simulationRunning;
    time_t lastUpdate;
    
    // System components
    vector<LogEntry> logs;
    int cruiseControlSpeed;
    bool cruiseControlActive;
    
    // Sensor status
    bool speedSensorOK;
    bool fuelSensorOK;
    bool brakeSensorOK;
    
    // Graphics
    TerminalGraphics graphics;
    
    // Animation counter
    int frameCount;
    
public:
    // Constructor
    SmartCarSystem() {
        speed = 60;
        fuel = 45;
        engineOn = true;
        warning = "Low Fuel";
        brakePressure = 100;
        throttle = 30;
        faultActive = false;
        faultCode = "";
        simulationRunning = true;
        lastUpdate = time(nullptr);
        cruiseControlSpeed = 60;
        cruiseControlActive = false;
        speedSensorOK = true;
        fuelSensorOK = true;
        brakeSensorOK = true;
        frameCount = 0;
        
        addLog("System initialized");
        addLog("Engine started");
        addLog("Warning: Low fuel detected");
    }
    
    // Add log entry
    void addLog(const string& message) {
        logs.push_back(LogEntry(message));
        if (logs.size() > 50) {
            logs.erase(logs.begin());
        }
    }
    
    // Clear screen
    void clearScreen() {
        system(CLEAR_SCREEN);
    }
    
    // Display header
    void displayHeader() {
        cout << Color::CYAN << Color::BOLD 
             << "════════════════════════════════════════════════════════════════" 
             << Color::RESET << endl;
        cout << Color::CYAN << Color::BOLD 
             << "           SMART CAR EMBEDDED SYSTEM SIMULATION v4.0" 
             << Color::RESET << endl;
        cout << Color::CYAN << Color::BOLD 
             << "════════════════════════════════════════════════════════════════" 
             << Color::RESET << endl;
        cout << endl;
    }
    
    // Display menu
    void displayMenu() {
        cout << Color::YELLOW << Color::BOLD << "┌─────────────────────────────────────────┐" << Color::RESET << endl;
        cout << Color::YELLOW << Color::BOLD << "│            CONTROL MENU                  │" << Color::RESET << endl;
        cout << Color::YELLOW << Color::BOLD << "├─────────────────────────────────────────┤" << Color::RESET << endl;
        cout << "│  " << Color::CYAN << "1." << Color::RESET << " Accelerate (+10 km/h)                  │" << endl;
        cout << "│  " << Color::CYAN << "2." << Color::RESET << " Brake (-10 km/h)                        │" << endl;
        cout << "│  " << Color::CYAN << "3." << Color::RESET << " Refuel (+10%)                           │" << endl;
        cout << "│  " << Color::CYAN << "4." << Color::RESET << " Toggle Engine                           │" << endl;
        cout << "│  " << Color::CYAN << "5." << Color::RESET << " Emergency Brake                         │" << endl;
        cout << "│  " << Color::CYAN << "6." << Color::RESET << " System Diagnostics                      │" << endl;
        cout << "│  " << Color::CYAN << "7." << Color::RESET << " View System Logs                        │" << endl;
        cout << "│  " << Color::CYAN << "8." << Color::RESET << " Clear Faults                            │" << endl;
        cout << "│  " << Color::CYAN << "9." << Color::RESET << " Toggle Cruise Control                   │" << endl;
        cout << "│  " << Color::CYAN << "10." << Color::RESET << " Simulate Sensor Failure                 │" << endl;
        cout << "│  " << Color::CYAN << "0." << Color::RESET << " Exit Simulation                         │" << endl;
        cout << Color::YELLOW << Color::BOLD << "└─────────────────────────────────────────┘" << Color::RESET << endl;
        cout << endl;
    }
    
    // Display system logs
    void displayLogs() {
        clearScreen();
        displayHeader();
        cout << Color::BLUE << Color::BOLD 
             << "━━━━━━━━━━━━━━━━━━━━━ SYSTEM LOGS ━━━━━━━━━━━━━━━━━━━━━" 
             << Color::RESET << endl;
        cout << endl;
        
        if (logs.empty()) {
            cout << Color::YELLOW << "No logs available." << Color::RESET << endl;
        } else {
            for (const auto& log : logs) {
                cout << Color::CYAN << log.toString() << Color::RESET << endl;
            }
        }
        
        cout << endl;
        cout << Color::YELLOW << "Press Enter to return to main dashboard..." << Color::RESET;
        cin.ignore();
        cin.get();
    }
    
    // Display diagnostics
    void displayDiagnostics() {
        clearScreen();
        displayHeader();
        cout << Color::BLUE << Color::BOLD 
             << "━━━━━━━━━━━━━━━━━━━ SYSTEM DIAGNOSTICS ━━━━━━━━━━━━━━━━━━━" 
             << Color::RESET << endl;
        cout << endl;
        
        cout << Color::BOLD << "Engine Control Unit (ECU):" << Color::RESET << endl;
        cout << "  - Engine Status: " << (engineOn ? "RUNNING" : "OFF") << endl;
        cout << "  - Throttle Position: " << throttle << "%" << endl;
        cout << "  - RPM: " << (speed * 30) << " RPM" << endl;
        cout << endl;
        
        cout << Color::BOLD << "Fuel Management System:" << Color::RESET << endl;
        cout << "  - Fuel Level: " << fuel << "%" << endl;
        cout << "  - Estimated Range: " << (fuel * 12) << " km" << endl;
        cout << "  - Fuel Sensor: " << (fuelSensorOK ? Color::GREEN + "OK" : Color::RED + "FAILED") << Color::RESET << endl;
        cout << endl;
        
        cout << Color::BOLD << "Brake Control System:" << Color::RESET << endl;
        cout << "  - Brake Pressure: " << brakePressure << "%" << endl;
        cout << "  - Brake Wear: " << (100 - brakePressure) << "%" << endl;
        cout << "  - Brake Sensor: " << (brakeSensorOK ? Color::GREEN + "OK" : Color::RED + "FAILED") << Color::RESET << endl;
        cout << endl;
        
        cout << Color::BOLD << "Sensors:" << Color::RESET << endl;
        cout << "  - Speed Sensor: " << speed << " km/h (" 
             << (speedSensorOK ? Color::GREEN + "Operational" : Color::RED + "FAILED") << Color::RESET << ")" << endl;
        cout << "  - Temperature: " << (rand() % 30 + 70) << "°C" << endl;
        cout << "  - Battery Voltage: " << (12.0 + (rand() % 30) / 10.0) << "V" << endl;
        cout << endl;
        
        cout << Color::BOLD << "Active Systems:" << Color::RESET << endl;
        cout << "  - Cruise Control: " << (cruiseControlActive ? "ACTIVE" : "INACTIVE") << endl;
        if (cruiseControlActive) {
            cout << "  - Cruise Set Speed: " << cruiseControlSpeed << " km/h" << endl;
        }
        cout << endl;
        
        cout << Color::BOLD << "Fault Memory:" << Color::RESET << endl;
        if (faultActive) {
            cout << "  - Active Fault: " << Color::RED << faultCode << Color::RESET << endl;
        } else {
            cout << "  - " << Color::GREEN << "No active faults" << Color::RESET << endl;
        }
        
        cout << endl;
        cout << Color::YELLOW << "Press Enter to return to main dashboard..." << Color::RESET;
        cin.ignore();
        cin.get();
    }
    
    // Check and handle faults
    void checkFaults() {
        // Check low fuel
        if (fuel < 10) {
            warning = "Critical Low Fuel";
            if (!faultActive) {
                faultCode = "FUEL-001: Fuel level critically low";
                faultActive = true;
                addLog("FAULT DETECTED: " + faultCode);
            }
        } 
        else if (fuel < 25) {
            warning = "Low Fuel";
        } 
        else if (fuel >= 25) {
            if (warning == "Low Fuel" || warning == "Critical Low Fuel") {
                warning = "None";
            }
            if (faultCode == "FUEL-001: Fuel level critically low") {
                faultActive = false;
                faultCode = "";
                addLog("Fault cleared: Fuel level normalized");
            }
        }
        
        // Check engine overheating
        if (engineOn && speed > 120) {
            if (!faultActive && faultCode.empty()) {
                faultCode = "ENG-002: Engine overheating (High speed)";
                faultActive = true;
                addLog("FAULT DETECTED: " + faultCode);
            }
        } 
        else if (speed <= 100 && faultCode == "ENG-002: Engine overheating (High speed)") {
            faultActive = false;
            faultCode = "";
            addLog("Fault cleared: Engine temperature normalized");
        }
        
        // Check brake system
        if (brakePressure < 20) {
            if (!faultActive && faultCode.empty()) {
                faultCode = "BRK-003: Brake pressure critically low";
                faultActive = true;
                addLog("FAULT DETECTED: " + faultCode);
            }
        } 
        else if (brakePressure >= 20 && faultCode == "BRK-003: Brake pressure critically low") {
            faultActive = false;
            faultCode = "";
            addLog("Fault cleared: Brake pressure normalized");
        }
        
        // Sensor failure checks
        if (!speedSensorOK) {
            if (!faultActive && faultCode.empty()) {
                faultCode = "SNS-004: Speed sensor failure";
                faultActive = true;
                addLog("FAULT DETECTED: " + faultCode);
            }
        } else if (faultCode == "SNS-004: Speed sensor failure") {
            faultActive = false;
            faultCode = "";
            addLog("Fault cleared: Speed sensor restored");
        }
        
        if (!fuelSensorOK) {
            warning = "Fuel Sensor Fault";
        } else if (warning == "Fuel Sensor Fault" && fuel >= 25) {
            warning = "None";
        }
    }
    
    // Real-time simulation update
    void realTimeUpdate() {
        time_t currentTime = time(nullptr);
        double timeDiff = difftime(currentTime, lastUpdate);
        
        if (timeDiff >= 2.0) {
            if (engineOn) {
                // Fuel consumption
                int consumption = (throttle / 10) + 1;
                fuel -= consumption;
                
                // Cruise control logic
                if (cruiseControlActive) {
                    if (speed < cruiseControlSpeed) {
                        throttle = min(80, throttle + 5);
                    } else if (speed > cruiseControlSpeed) {
                        throttle = max(10, throttle - 5);
                    }
                } else {
                    // Random throttle variation
                    throttle = rand() % 50 + 20;
                }
                
                // Speed variation based on throttle
                if (throttle > 40) {
                    speed += 2;
                } else if (throttle < 20) {
                    speed -= 1;
                }
                
                // Keep speed within bounds
                if (speed < 0) speed = 0;
                if (speed > 180) {
                    speed = 180;
                    addLog("Warning: Maximum speed reached");
                }
                
                // Keep fuel within bounds
                if (fuel <= 0) {
                    fuel = 0;
                    engineOn = false;
                    addLog("EMERGENCY: Engine stopped due to fuel depletion");
                }
                if (fuel > 100) fuel = 100;
            } 
            else {
                // Engine off - coasting to stop
                if (speed > 0) {
                    speed -= 2;
                    if (speed < 0) speed = 0;
                }
                throttle = 0;
                cruiseControlActive = false;
            }
            
            // Brake pressure recovery
            if (brakePressure < 100) {
                brakePressure += 5;
                if (brakePressure > 100) brakePressure = 100;
            }
            
            lastUpdate = currentTime;
            checkFaults();
        }
    }
    
    // Accelerate
    void accelerate() {
        if (engineOn) {
            speed += 10;
            if (speed > 180) speed = 180;
            addLog("Accelerated to " + to_string(speed) + " km/h");
            if (cruiseControlActive) {
                cruiseControlActive = false;
                addLog("Cruise control disabled due to manual acceleration");
            }
        } else {
            cout << Color::RED << "Cannot accelerate: Engine is OFF!" << Color::RESET << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
    
    // Brake
    void brake() {
        if (engineOn || speed > 0) {
            int oldSpeed = speed;
            speed -= 10;
            if (speed < 0) speed = 0;
            brakePressure -= 15;
            if (brakePressure < 0) brakePressure = 0;
            addLog("Braked from " + to_string(oldSpeed) + " to " + to_string(speed) + " km/h");
            if (cruiseControlActive) {
                cruiseControlActive = false;
                addLog("Cruise control disabled due to braking");
            }
        }
    }
    
    // Emergency brake
    void emergencyBrake() {
        if (speed > 0) {
            addLog("EMERGENCY BRAKE ACTIVATED!");
            speed = 0;
            brakePressure = 40;
            addLog("Vehicle stopped immediately");
            if (cruiseControlActive) {
                cruiseControlActive = false;
                addLog("Cruise control disabled");
            }
        }
    }
    
    // Refuel
    void refuel() {
        int oldFuel = fuel;
        fuel += 10;
        if (fuel > 100) fuel = 100;
        addLog("Refueled from " + to_string(oldFuel) + "% to " + to_string(fuel) + "%");
        if (fuel > 25) {
            warning = "None";
        }
    }
    
    // Toggle engine
    void toggleEngine() {
        engineOn = !engineOn;
        if (engineOn) {
            addLog("Engine started");
        } else {
            addLog("Engine stopped");
            cruiseControlActive = false;
        }
    }
    
    // Clear faults
    void clearFaults() {
        if (faultActive) {
            addLog("Manually cleared fault: " + faultCode);
            faultActive = false;
            faultCode = "";
            cout << Color::GREEN << "Faults cleared successfully!" << Color::RESET << endl;
        } else {
            cout << Color::YELLOW << "No active faults to clear." << Color::RESET << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    
    // Toggle cruise control
    void toggleCruiseControl() {
        if (!engineOn) {
            cout << Color::RED << "Cannot activate cruise control: Engine is OFF!" << Color::RESET << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
            return;
        }
        
        if (!cruiseControlActive) {
            cruiseControlActive = true;
            cruiseControlSpeed = speed;
            addLog("Cruise control activated at " + to_string(cruiseControlSpeed) + " km/h");
            cout << Color::GREEN << "Cruise control activated at " << cruiseControlSpeed << " km/h" << Color::RESET << endl;
        } else {
            cruiseControlActive = false;
            addLog("Cruise control deactivated");
            cout << Color::YELLOW << "Cruise control deactivated" << Color::RESET << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    
    // Simulate sensor failure
    void simulateSensorFailure() {
        clearScreen();
        displayHeader();
        cout << Color::YELLOW << Color::BOLD << "SELECT SENSOR TO FAIL:" << Color::RESET << endl;
        cout << Color::CYAN << "1." << Color::RESET << " Speed Sensor" << endl;
        cout << Color::CYAN << "2." << Color::RESET << " Fuel Sensor" << endl;
        cout << Color::CYAN << "3." << Color::RESET << " Brake Sensor" << endl;
        cout << Color::CYAN << "4." << Color::RESET << " Reset All Sensors" << endl;
        cout << endl;
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1:
                speedSensorOK = !speedSensorOK;
                addLog(string("Speed sensor ") + (speedSensorOK ? "restored" : "failed"));
                cout << (speedSensorOK ? Color::GREEN : Color::RED) 
                     << "Speed sensor " << (speedSensorOK ? "operational" : "failed") 
                     << Color::RESET << endl;
                break;
            case 2:
                fuelSensorOK = !fuelSensorOK;
                addLog(string("Fuel sensor ") + (fuelSensorOK ? "restored" : "failed"));
                cout << (fuelSensorOK ? Color::GREEN : Color::RED) 
                     << "Fuel sensor " << (fuelSensorOK ? "operational" : "failed") 
                     << Color::RESET << endl;
                break;
            case 3:
                brakeSensorOK = !brakeSensorOK;
                addLog(string("Brake sensor ") + (brakeSensorOK ? "restored" : "failed"));
                cout << (brakeSensorOK ? Color::GREEN : Color::RED) 
                     << "Brake sensor " << (brakeSensorOK ? "operational" : "failed") 
                     << Color::RESET << endl;
                break;
            case 4:
                speedSensorOK = true;
                fuelSensorOK = true;
                brakeSensorOK = true;
                addLog("All sensors reset to operational status");
                cout << Color::GREEN << "All sensors restored to operational status" << Color::RESET << endl;
                break;
        }
        this_thread::sleep_for(chrono::seconds(2));
    }
    
    // Run main simulation loop with live graphics
    void run() {
        int choice;
        
        while (simulationRunning) {
            clearScreen();
            displayHeader();
            
            // Draw live graphics dashboard
            graphics.drawLiveFrame(speed, fuel, engineOn, throttle, brakePressure, 
                                   warning, faultActive, faultCode);
            
            displayMenu();
            
            realTimeUpdate();
            
            cout << "\nEnter your choice: ";
            cin >> choice;
            
            switch(choice) {
                case 1:
                    accelerate();
                    break;
                case 2:
                    brake();
                    break;
                case 3:
                    refuel();
                    break;
                case 4:
                    toggleEngine();
                    break;
                case 5:
                    emergencyBrake();
                    break;
                case 6:
                    displayDiagnostics();
                    break;
                case 7:
                    displayLogs();
                    break;
                case 8:
                    clearFaults();
                    break;
                case 9:
                    toggleCruiseControl();
                    break;
                case 10:
                    simulateSensorFailure();
                    break;
                case 0:
                    simulationRunning = false;
                    addLog("System shutdown");
                    cout << Color::GREEN << "\nThank you for using Smart Car Simulation!" << Color::RESET << endl;
                    break;
                default:
                    cout << Color::RED << "Invalid choice! Please try again." << Color::RESET << endl;
                    this_thread::sleep_for(chrono::milliseconds(500));
                    break;
            }
        }
    }
};

// Main function
int main() {
    srand(time(nullptr));
    
    SmartCarSystem car;
    car.run();
    
    return 0;
}

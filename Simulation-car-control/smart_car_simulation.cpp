// smart_car_simulation.cpp
// Smart Car Embedded System Simulation - Complete C++ Implementation

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

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
#else
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
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
    const string BOLD = "\033[1m";
    const string RESET = "\033[0m";
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
             << "           SMART CAR EMBEDDED SYSTEM SIMULATION v3.0" 
             << Color::RESET << endl;
        cout << Color::CYAN << Color::BOLD 
             << "════════════════════════════════════════════════════════════════" 
             << Color::RESET << endl;
        cout << endl;
    }
    
    // Display main dashboard
    void displayDashboard() {
        cout << Color::BLUE << Color::BOLD 
             << "━━━━━━━━━━━━━━━━━━━━━ MAIN DASHBOARD ━━━━━━━━━━━━━━━━━━━━━" 
             << Color::RESET << endl;
        cout << endl;
        
        // Speed display
        string speedColor;
        if (speed < 40) speedColor = Color::GREEN;
        else if (speed < 100) speedColor = Color::YELLOW;
        else speedColor = Color::RED;
        
        cout << Color::BOLD << "Speed:" << Color::RESET << "      " 
             << speedColor << speed << " km/h" << Color::RESET;
        if (cruiseControlActive) {
            cout << " " << Color::CYAN << "[CRUISE: " << cruiseControlSpeed << "]" << Color::RESET;
        }
        cout << endl;
        
        // Fuel display
        string fuelColor;
        if (fuel < 15) fuelColor = Color::RED;
        else if (fuel < 30) fuelColor = Color::YELLOW;
        else fuelColor = Color::GREEN;
        
        cout << Color::BOLD << "Fuel:" << Color::RESET << "        " 
             << fuelColor << fuel << "%" << Color::RESET << endl;
        
        // Engine status
        string engineColor = engineOn ? Color::GREEN : Color::RED;
        cout << Color::BOLD << "Engine:" << Color::RESET << "      " 
             << engineColor << (engineOn ? "ON" : "OFF") << Color::RESET << endl;
        
        // Warning display
        if (warning != "None") {
            cout << Color::BOLD << "Warning:" << Color::RESET << "     " 
                 << Color::RED << "⚠ " << warning << Color::RESET << endl;
        } else {
            cout << Color::BOLD << "Warning:" << Color::RESET << "     " 
                 << Color::GREEN << "✓ None" << Color::RESET << endl;
        }
        
        // Fault display
        if (faultActive) {
            cout << Color::BOLD << "Fault:" << Color::RESET << "        " 
                 << Color::RED << "🔧 " << faultCode << Color::RESET << endl;
        } else {
            cout << Color::BOLD << "Fault:" << Color::RESET << "        " 
                 << Color::GREEN << "✓ No faults" << Color::RESET << endl;
        }
        
        // Brake system
        string brakeColor;
        if (brakePressure < 30) brakeColor = Color::RED;
        else if (brakePressure < 70) brakeColor = Color::YELLOW;
        else brakeColor = Color::GREEN;
        
        cout << Color::BOLD << "Brake Pressure:" << Color::RESET << " " 
             << brakeColor << brakePressure << "%" << Color::RESET << endl;
        
        // Throttle
        cout << Color::BOLD << "Throttle:" << Color::RESET << "     " 
             << throttle << "%" << endl;
        
        cout << endl;
        cout << Color::BLUE << Color::BOLD 
             << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" 
             << Color::RESET << endl;
    }
    
    // Display menu
    void displayMenu() {
        cout << Color::YELLOW << Color::BOLD << "CONTROL MENU:" << Color::RESET << endl;
        cout << Color::CYAN << "1." << Color::RESET << " Accelerate (+10 km/h)" << endl;
        cout << Color::CYAN << "2." << Color::RESET << " Brake (-10 km/h)" << endl;
        cout << Color::CYAN << "3." << Color::RESET << " Refuel (+10%)" << endl;
        cout << Color::CYAN << "4." << Color::RESET << " Toggle Engine" << endl;
        cout << Color::CYAN << "5." << Color::RESET << " Emergency Brake" << endl;
        cout << Color::CYAN << "6." << Color::RESET << " System Diagnostics" << endl;
        cout << Color::CYAN << "7." << Color::RESET << " View System Logs" << endl;
        cout << Color::CYAN << "8." << Color::RESET << " Clear Faults" << endl;
        cout << Color::CYAN << "9." << Color::RESET << " Toggle Cruise Control" << endl;
        cout << Color::CYAN << "10." << Color::RESET << " Simulate Sensor Failure" << endl;
        cout << Color::CYAN << "0." << Color::RESET << " Exit Simulation" << endl;
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
        }
        
        if (!fuelSensorOK) {
            warning = "Fuel Sensor Fault";
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
            this_thread::sleep_for(chrono::seconds(1));
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
        this_thread::sleep_for(chrono::seconds(1));
    }
    
    // Toggle cruise control
    void toggleCruiseControl() {
        if (!engineOn) {
            cout << Color::RED << "Cannot activate cruise control: Engine is OFF!" << Color::RESET << endl;
            this_thread::sleep_for(chrono::seconds(1));
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
        this_thread::sleep_for(chrono::seconds(1));
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
    
    // Run main simulation loop
    void run() {
        int choice;
        
        while (simulationRunning) {
            clearScreen();
            displayHeader();
            displayDashboard();
            displayMenu();
            
            realTimeUpdate();
            
            cout << "Enter your choice: ";
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
                    this_thread::sleep_for(chrono::seconds(1));
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

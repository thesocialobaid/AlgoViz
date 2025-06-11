🔄 Sorting Visualizer – C++ Algorithm Animation Tool

## Overview 
Build an interactive sorting visualizer that animates the behaviour of sorting classic sorting algorithms- Bubble Sort, Insertion Sort and Selection Sort. Designed to help 
learners understand the inner workings of sorting through intiutive real time graphical representation. Emphasizes modular code structure, UI responsiveness and clean visual 
feedback. 

## 🧰 Tech Stack: 

 C++ – Core programming language for logic and performance
 SFML (Simple and Fast Multimedia Library) – Used for rendering graphics, managing UI events, and handling window operations
 OOP Principles – Applied to separate data structures, visual elements, and algorithm logic for better maintainability

## 🧩 System Architecture: 

The system is structured into distinct modules to support scalability and maintainability:

1. Input & Initialization → 2. Visualization Engine → 3. Sorting Algorithm Logic → 4. UI Rendering

Input & Initialization: Initializes the graphical window, loads assets (fonts, window size), and sets up the initial data array.

Visualization Engine: Translates array states into dynamic bar representations using SFML, adjusting height, color, and position in real-time.

Sorting Logic: Algorithms run step-by-step with visual cues, including comparisons, swaps, and sorted segments. Thread synchronization ensures rendering and logic stay aligned.

UI Rendering: SFML handles frame updates, user input (like sorting method selection or restart), and smooth visual transitions.




#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <functional>
using namespace std;
using namespace sf;


class Array {
private:
    int* values;
    int size;
public:
    Array(int s) {
        size = s;
        values = new int[size];
    }

   
    void randomize() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(10, 50);
        for (int i = 0; i < size; i++) {
            values[i] = distr(gen);
        }
    }

    void reset() {
       
        for (int i = 0; i < size; i++) {
            values[i] = 0;
        }
        randomize();
    }

    
    void draw(RenderWindow& window, int highlighted1 = -1, int highlighted2 = -1,
        Font* font = nullptr, bool fontLoaded = false) {
        float barWidth = (float)window.getSize().x / size;

        for (int i = 0; i < size; i++) {
            RectangleShape bar;
            bar.setSize(Vector2f(barWidth - 1, values[i] * 10));

           
            if (i == highlighted1 || i == highlighted2) {
                bar.setFillColor(Color::Red);
            }
            else {
                bar.setFillColor(Color::White);
            }

            bar.setPosition(i * barWidth, window.getSize().y - bar.getSize().y);
            window.draw(bar);

            
            if (fontLoaded && font != nullptr) {
                Text valueText;
                valueText.setFont(*font);
                valueText.setString(to_string(values[i]));
                valueText.setCharacterSize(12);
                valueText.setFillColor(Color::White);
                FloatRect textBounds = valueText.getLocalBounds();
                valueText.setOrigin(textBounds.width / 2, textBounds.height);
                valueText.setPosition(i * barWidth + barWidth / 2,
                    window.getSize().y - bar.getSize().y - 5);
                window.draw(valueText);
            }
        }
    }

    void swap(int i, int j) {
        int temp = values[i];
        values[i] = values[j];
        values[j] = temp;
    }

    int get_value(int i) { return values[i]; }

    void set(int i, int value) { values[i] = value; }

    int getSize() { return size; }

    ~Array() {
        delete[] values;
    }
};


class Sorter {
protected:
    Array& array;
    int speed; 
    bool isSorting;
    function<void(int, int)> updateCallback;

public:
    Sorter(Array& arr) : array(arr), speed(100), isSorting(false) {}

    void setSpeed(int s) { speed = s; }

    bool getIsSorting() const { return isSorting; }

    void setIsSorting(bool value) { isSorting = value; }

    void setUpdateCallback(function<void(int, int)> callback) {
        updateCallback = callback;
    }

    
    virtual string getAlgorithmName() const = 0;
    virtual string getTimeComplexity() const = 0;
    virtual string getSpaceComplexity() const = 0;
    virtual void sort() = 0;

    virtual ~Sorter() {} 
};


class BubbleSort : public Sorter {
public:
    BubbleSort(Array& arr) : Sorter(arr) {}

    string getAlgorithmName() const override {
        return "Bubble Sort";
    }

    string getTimeComplexity() const override {
        return "O(n²)";
    }

    string getSpaceComplexity() const override {
        return "O(1)";
    }

    void sort() override {
        setIsSorting(true);

        
        for (int i = 0; i < array.getSize() - 1 && getIsSorting(); i++) {
            for (int j = 0; j < array.getSize() - 1 - i && getIsSorting(); j++) {
                if (updateCallback) updateCallback(j, j + 1);

                // Swap if needed
                if (array.get_value(j) > array.get_value(j + 1)) {
                    array.swap(j, j + 1);
                    if (updateCallback) updateCallback(j, j + 1);
                }

                this_thread::sleep_for(chrono::milliseconds(speed));
            }
        }

        if (updateCallback) updateCallback(-1, -1); 
        setIsSorting(false);
    }
};


class InsertionSort : public Sorter {
public:
    InsertionSort(Array& arr) : Sorter(arr) {}

    string getAlgorithmName() const override {
        return "Insertion Sort";
    }

    string getTimeComplexity() const override {
        return "O(n²)"; 
    }

    string getSpaceComplexity() const override {
        return "O(1)";
    }

    void sort() override {
        setIsSorting(true);

        for (int i = 1; i < array.getSize() && getIsSorting(); i++) {
            int key = array.get_value(i);
            int j = i - 1;

            if (updateCallback) updateCallback(i, -1);
            this_thread::sleep_for(chrono::milliseconds(speed));

            
            while (j >= 0 && array.get_value(j) > key && getIsSorting()) {
                if (updateCallback) updateCallback(j, j + 1);
                array.set(j + 1, array.get_value(j));
                j--;

                if (updateCallback) updateCallback(j + 1, -1);
                this_thread::sleep_for(chrono::milliseconds(speed));
            }

            array.set(j + 1, key);
            if (updateCallback) updateCallback(j + 1, -1);
            this_thread::sleep_for(chrono::milliseconds(speed));
        }

        if (updateCallback) updateCallback(-1, -1);
        setIsSorting(false);
    }
};


class SelectionSort : public Sorter {
public:
    SelectionSort(Array& arr) : Sorter(arr) {}

    string getAlgorithmName() const override {
        return "Selection Sort";
    }

    string getTimeComplexity() const override {
        return "O(n²)";
    }

    string getSpaceComplexity() const override {
        return "O(1)";
    }

    void sort() override {
        setIsSorting(true);

        
        for (int i = 0; i < array.getSize() - 1 && getIsSorting(); i++) {
            int minIndex = i;

            if (updateCallback) updateCallback(i, -1);
            this_thread::sleep_for(chrono::milliseconds(speed));

            for (int j = i + 1; j < array.getSize() && getIsSorting(); j++) {
                if (updateCallback) updateCallback(minIndex, j);
                this_thread::sleep_for(chrono::milliseconds(speed));

                if (array.get_value(j) < array.get_value(minIndex)) {
                    minIndex = j;
                }
            }

           
            if (updateCallback) updateCallback(i, minIndex);
            this_thread::sleep_for(chrono::milliseconds(speed));

            array.swap(i, minIndex);

            if (updateCallback) updateCallback(i, minIndex);
            this_thread::sleep_for(chrono::milliseconds(speed));
        }

        if (updateCallback) updateCallback(-1, -1);
        setIsSorting(false);
    }
};

class Visualizer {
private:
    RenderWindow window;
    Array array;
    Sorter* sorter;
    int speed;
    bool sorting;
    int highlight1;
    int highlight2;
    thread sortingThread;
    Font font;
    bool fontLoaded;

public:
    Visualizer(int windowWidth, int windowHeight, int arraySize)
        : window(VideoMode(windowWidth, windowHeight), "ALGOVIZ-SORTING VISUALIZER"),
        array(arraySize), sorting(false), speed(100), sorter(nullptr),
        highlight1(-1), highlight2(-1) {

        array.reset();

        
        fontLoaded = font.loadFromFile("arial.ttf");

        if (!fontLoaded)
            fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");

        if (!fontLoaded) 
            fontLoaded = font.loadFromFile("C:/Windows/Fonts/segoeui.ttf");

        if (!fontLoaded) 
            fontLoaded = font.loadFromFile("/usr/share/fonts/truetype/freefont/FreeSans.ttf");

        if (!fontLoaded) 
            fontLoaded = font.loadFromFile("/Library/Fonts/Arial.ttf");

        if (!fontLoaded) {
            cout << "Warning: Could not load font. UI text may not display correctly." << endl;
            cout << "Algorithm details will be displayed in console." << endl;
        }
    }

    void setSorter(Sorter* newSorter) {
        stopSorting();

        if (sorter != nullptr) {
            delete sorter;
        }

        sorter = newSorter;

        if (sorter != nullptr) {
            
            cout << "\n---------------------------------" << endl;
            cout << "Algorithm: " << sorter->getAlgorithmName() << endl;
            cout << "Time Complexity: " << sorter->getTimeComplexity() << endl;
            cout << "Space Complexity: " << sorter->getSpaceComplexity() << endl;
            cout << "---------------------------------\n" << endl;

          
            sorter->setUpdateCallback([this](int h1, int h2) {
                highlight1 = h1;
                highlight2 = h2;
                });
        }
    }

    void stopSorting() {
        if (sorting && sortingThread.joinable()) {
            if (sorter != nullptr) {
                sorter->setIsSorting(false);
            }
            sortingThread.join();
            sorting = false;
        }
    }

    void startSort() {
        if (sorter != nullptr && !sorting) {
            sorting = true;
            sortingThread = thread([this]() {
                sorter->sort();
                sorting = false;
                });
            sortingThread.detach(); 
        }
    }

    void resetArray() {
        stopSorting();
        array.reset();
        highlight1 = -1;
        highlight2 = -1;
    }

    void update() {
        
        if (sorter != nullptr && !sorter->getIsSorting() && sorting) {
            sorting = false;
        }
    }

    void draw() {
        window.clear(Color(30, 30, 30)); 

        
        array.draw(window, highlight1, highlight2, &font, fontLoaded);

        
        drawUIText("ALGOVIZ - SORTING VISUALIZER", 10, 10, 24);
        drawUIText("Press [S] to Start Sorting | [R] to Reset | [A] to Change Algorithm", 10, 40, 16);

       
        string algorithmText = "Current Algorithm: ";
        string complexityText = "Time Complexity: ";
        string spaceText = "Space Complexity: ";

        if (sorter != nullptr) {
            algorithmText += sorter->getAlgorithmName();
            complexityText += sorter->getTimeComplexity();
            spaceText += sorter->getSpaceComplexity();
        }
        else {
            algorithmText += "None";
            complexityText += "N/A";
            spaceText += "N/A";
        }

        drawUIText(algorithmText, 10, 65, 16);
        drawUIText(complexityText, 10, 90, 16);
        drawUIText(spaceText, 10, 115, 16);

        window.display();
    }

  
    void drawUIText(const string& text, float x, float y, unsigned int size) {
        if (fontLoaded) {
            Text textObj(text, font, size);
            textObj.setPosition(x, y);
            textObj.setFillColor(Color::White);
            window.draw(textObj);
        }
        else {
        
            RectangleShape infoBar(Vector2f(10, 10));
            infoBar.setFillColor(Color::White);
            infoBar.setPosition(x, y);
            window.draw(infoBar);
        }
    }

    RenderWindow& getWindow() {
        return window;
    }

    Array& getArray() {
        return array;
    }

   
    void chooseSortingAlgorithm() {
        RenderWindow menuWindow(VideoMode(600, 500), "Algorithm Selection");

       
        if (!fontLoaded) {
            menuWindow.close();

            cout << "\n\n  SORTING ALGORITHM SELECTION\n";
            cout << "  --------------------------\n";
            cout << "  1. Bubble Sort - O(n²)\n";
            cout << "  2. Insertion Sort - O(n²)\n";
            cout << "  3. Selection Sort - O(n²)\n";
            cout << "\n  Choose algorithm (1-3): ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                setSorter(new BubbleSort(array));
                break;
            case 2:
                setSorter(new InsertionSort(array));
                break;
            case 3:
                setSorter(new SelectionSort(array));
                break;
            default:
                setSorter(new BubbleSort(array));
                cout << "  Invalid choice! Defaulting to Bubble Sort.\n";
            }

            this_thread::sleep_for(chrono::milliseconds(1500));
            return;
        }

    
        Text title("SELECT SORTING ALGORITHM ", font, 24);
        title.setPosition(50, 30);
        title.setFillColor(Color::White);

      
        Text option1("1. Bubble Sort", font, 18);
        option1.setPosition(50, 100);
        option1.setFillColor(Color::White);

        Text bubble_complex("Time Complexity: O(n²) | Space Complexity: O(1)", font, 14);
        bubble_complex.setPosition(70, 125);
        bubble_complex.setFillColor(Color::Yellow);

      
        Text option2("2. Insertion Sort", font, 18);
        option2.setPosition(50, 180);
        option2.setFillColor(Color::White);

        Text insert_complex("Time Complexity: O(n²) | Space Complexity: O(1)", font, 14);
        insert_complex.setPosition(70, 205);
        insert_complex.setFillColor(Color::Yellow);

        
        Text option3("3. Selection Sort", font, 18);
        option3.setPosition(50, 260);
        option3.setFillColor(Color::White);

        Text select_complex("Time Complexity: O(n²) | Space Complexity: O(1)", font, 14);
        select_complex.setPosition(70, 285);
        select_complex.setFillColor(Color::Yellow);

        
        Text instruction("Press 1, 2, or 3 to select | ESC to cancel", font, 16);
        instruction.setPosition(50, 350);
        instruction.setFillColor(Color::Green);

       
        while (menuWindow.isOpen()) {
            Event event;
            while (menuWindow.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    menuWindow.close();
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Num1 || event.key.code == Keyboard::Numpad1) {
                        setSorter(new BubbleSort(array));
                        menuWindow.close();
                    }
                    else if (event.key.code == Keyboard::Num2 || event.key.code == Keyboard::Numpad2) {
                        setSorter(new InsertionSort(array));
                        menuWindow.close();
                    }
                    else if (event.key.code == Keyboard::Num3 || event.key.code == Keyboard::Numpad3) {
                        setSorter(new SelectionSort(array));
                        menuWindow.close();
                    }
                    else if (event.key.code == Keyboard::Escape) {
                        menuWindow.close();
                    }
                }
            }

         
            menuWindow.clear(Color(30, 30, 30));
            menuWindow.draw(title);
            menuWindow.draw(option1);
            menuWindow.draw(bubble_complex);
            menuWindow.draw(option2);
            menuWindow.draw(insert_complex);
            menuWindow.draw(option3);
            menuWindow.draw(select_complex);
            menuWindow.draw(instruction);
            menuWindow.display();
        }
    }

    ~Visualizer() {
        stopSorting();
        delete sorter; 
    }
};


class Controller {
private:
    Visualizer viz;
public:
    Controller(int windowWidth, int windowHeight, int arraySize)
        : viz(windowWidth, windowHeight, arraySize) {
    }

    Visualizer& getVisualizer() {
        return viz;
    }

    void handleEvents() {
        Event event;
        while (viz.getWindow().pollEvent(event)) {
            if (event.type == Event::Closed) {
                viz.getWindow().close();
            }

            
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::S) {
                    viz.startSort();
                }
                else if (event.key.code == Keyboard::R) {
                    viz.resetArray();
                }
                else if (event.key.code == Keyboard::A) {
                    viz.chooseSortingAlgorithm();
                }
            }
        }
    }

    void run() {
      
        viz.setSorter(new BubbleSort(viz.getArray()));

     
        cout << "\nALGOVIZ - SORTING VISUALIZER" << endl;
        cout << "==========================" << endl;
        cout << "Press [S] to Start Sorting" << endl;
        cout << "Press [R] to Reset Array" << endl;
        cout << "Press [A] to Change Algorithm" << endl;
        cout << "==========================" << endl;

        
        while (viz.getWindow().isOpen()) {
            handleEvents();
            viz.update();
            viz.draw();
            sleep(milliseconds(16)); 
        }
    }
};


int main() {
 
    Controller controller(800, 600, 50);
    controller.run();
    return 0;
}


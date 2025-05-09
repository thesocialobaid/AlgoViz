#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <functional>
using namespace std;
using namespace sf;


class Array {
private:
    int* values;
    int size;
    const int MIN_height = 10; 
    const int MAX_height = 50; 
    const int Multiplier = 10; // Is ko ham use karien ge for drawing purposes. 

public:
    Array(int s) {
        size = s;
        values = new int[size];
    }

   // Yeh C ke through aik standard tareeqa hai to initalize the array to random values. 
    void randomize() {
        srand(time(NULL)); 
        for (int i = 0; i < size; ++i) {
            values[i] = MIN_height + rand() % (MAX_height - MIN_height); 
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
                bar.setFillColor(Color::Red); // I have used HSV Values to ensure that the colors can be changed in the future 
            }
            else {
                bar.setFillColor(Color::White);
            }

            bar.setPosition(i * barWidth, window.getSize().y - bar.getSize().y);
            window.draw(bar); //This allows us to draw the bars itself. 

            // Is sare code keh through ham log array ki values ko show kara rahe hain. 
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
        //Yeh aik simple swap ka function hai jis keh through values swap hoongi 
        int temp = values[i];
        values[i] = values[j];
        values[j] = temp;
    }
    //Getter to get the value of the specfic index position 
    int get_value(int i) { 
        return values[i];
    }
    //Setter to set the value at the specific index position 
    void set(int i, int value) { 
        values[i] = value; 
    }

    //Getter to get the size of the array 
    int getSize() { 
        return size;
    }

    ~Array() {
        delete[] values;
        values = nullptr; //Dynamic deallocation and referencing the array to the null pointer. 
    }
};

//Yeh ensure karne ke liye keh UI Freeze na ho, ham log update call back use kar rahe hain. 
// An update call back refreshes the user interface dynamically 
typedef void(*UpdateFn)(int, int); 


class SorterAlgorithm {
protected:
    Array& array;
    int speed; 
    bool isSorting;
    UpdateFn updateCallbackFn; //Function pointer is used to call the update back function.


public:
    SorterAlgorithm(Array& arr): array(arr){ 
        speed = 100; 
        isSorting = false; 
        updateCallbackFn = nullptr; 
         }

    //Get function to check if sorting is being carried out or not. 
    bool getIsSorting() const { 
        return isSorting; 
    }

    //Setter to set the state of the sorting being done 
    void setIsSorting(bool value) { 
        isSorting = value; 
    }

    void setSpeed(int speed) {
        this->speed = speed; 
    }


    void setCallback(UpdateFn fn) {
        updateCallbackFn = fn; 
    }

    

    //YEH HAMARAY PURE VIRTUAL FUNCTONS HAIN JO KE RUN TIME POLYMORPHISM ENSURE KARIEN GE! 
    virtual string getAlgoName() const = 0;
    virtual string getT_C() const = 0;
    virtual string getS_C() const = 0;
    virtual void sort() = 0;

    virtual ~SorterAlgorithm() {} 
};


class BubbleSort : public SorterAlgorithm {
public:
    BubbleSort(Array& arr) : SorterAlgorithm(arr) {}

    string getAlgoName() const override {
        return "Bubble Sort";
    }

    string getT_C() const override {
        return " O ( n² )";
    }

    string getS_C() const override {
        return " O ( 1 ) ";
    }
    // Yahan par basic sorting function run time polymorphism keh through over-ride honay lag jayega! 
    void sort() override {
        setIsSorting(true);

        
        for (int i = 0; i < array.getSize() - 1 && getIsSorting(); i++) {
            for (int j = 0; j < array.getSize() - 1 - i && getIsSorting(); j++) {
                if (updateCallbackFn) updateCallbackFn(j, j + 1);

                // Swap if needed
                if (array.get_value(j) > array.get_value(j + 1)) {
                    array.swap(j, j + 1);
                    if (updateCallbackFn) updateCallbackFn(j, j + 1);
                }
               
                //As the code is going to be running the sleep will vary slightly with every iteration! 
                this_thread::sleep_for(chrono::milliseconds(speed+ (rand() % 3 )));
            }
        }

        if (updateCallbackFn) updateCallbackFn(-1, -1); 
        setIsSorting(false);
    }
};


class InsertionSort : public SorterAlgorithm {

public:
    InsertionSort(Array& arr) : SorterAlgorithm(arr) {}

    string getAlgoName() const override {
        return " Insertion Sort ";
    }

    string getT_C() const override {
        return " O ( n² ) "; 
    }

    string getS_C() const override {
        return " O ( 1 ) ";
    }

    void sort() override {
        setIsSorting(true);

        for (int i = 1; i < array.getSize() && getIsSorting(); i++) {
            int key = array.get_value(i);
            int j = i - 1;

            if (updateCallbackFn) updateCallbackFn(i, -1);
            this_thread::sleep_for(chrono::milliseconds(speed));

            
            while (j >= 0 && array.get_value(j) > key && getIsSorting()) {
                if (updateCallbackFn) updateCallbackFn(j, j + 1);
                array.set(j + 1, array.get_value(j));
                --j; //Yahan par ham aik prefix decrement use kar rahe hain 

                if (updateCallbackFn) updateCallbackFn(j + 1, -1);
                this_thread::sleep_for(chrono::milliseconds(speed));
            }

            array.set(j + 1, key);

            if (updateCallbackFn) updateCallbackFn(j + 1, -1);
            this_thread::sleep_for(chrono::milliseconds(speed));
        }

        if (updateCallbackFn) updateCallbackFn(-1, -1);
        setIsSorting(false);
    }
};


class SelectionSort : public SorterAlgorithm {
public:
    SelectionSort(Array& arr) : SorterAlgorithm(arr) {}

    string getAlgoName() const override {
        return " Selection Sort ";
    }

    string getT_C() const override {
        return " O ( n² ) ";
    }

    string getS_C() const override {
        return " O ( 1 ) ";
    }

    void sort() override {
        setIsSorting(true);

        // Possible optimization could be to possibly tackle empty arrays, but this will not be possible since there's a very hard chance keh koi empty array ayegi. 
        for (int i = 0; i < array.getSize() - 1 && getIsSorting(); i++) {
            int minIndex = i;

            if (updateCallbackFn) updateCallbackFn(i, -1);
            this_thread::sleep_for(chrono::milliseconds(speed));

            for (int j = i + 1; j < array.getSize() && getIsSorting(); j++) {
                if (updateCallbackFn) updateCallbackFn(minIndex, j);
                this_thread::sleep_for(chrono::milliseconds(speed));

                if (array.get_value(j) < array.get_value(minIndex)) {
                    minIndex = j;
                }
            }

           
            if (updateCallbackFn) updateCallbackFn(i, minIndex);
            this_thread::sleep_for(chrono::milliseconds(speed));

            array.swap(i, minIndex);

            if (updateCallbackFn) updateCallbackFn(i, minIndex);
            this_thread::sleep_for(chrono::milliseconds(speed));
        }

        if (updateCallbackFn) updateCallbackFn(-1, -1);
        setIsSorting(false);
    }
};

int global_highlight1 = -1;
int global_highlight2 = -1;

void updateHighlightsCallback(int h1, int h2) {
    global_highlight1 = h1;
    global_highlight2 = h2;
}

// Is class ke through ham log saari visualization and UI Waighera ko sort out karien ge
// 

class Visualizer {
private:
    sf::RenderWindow window;
    Array array;
    SorterAlgorithm* sorter;
    int speed;
    bool sorting;
    int highlight1;
    int highlight2;
    thread sortingThread; // Thread keh through ham effective management ko consider kar rahe hain. 
    sf::Font font;           
    bool fontLoaded;         

public:
   //Aik parametrized constructor values ko initialize karne ke liye. 
    Visualizer(int windowWidth, int windowHeight, int arraySize)
        : window(sf::VideoMode(windowWidth, windowHeight), "ALGOVIZ-Sorting Visualizer "),
        array(arraySize), sorting(false), speed(100), sorter(nullptr),
        highlight1(-1), highlight2(-1) {

     
        array.reset();

        //Call back and escalation method agar font load nahi hota tou! 
    
        fontLoaded = font.loadFromFile("arial.ttf");

        if (!fontLoaded) {
          
            fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");
        }

        if (!fontLoaded) {
            // Try Windows alternate location
            fontLoaded = font.loadFromFile("C:/Windows/Fonts/segoeui.ttf");
        }


        // If still not loaded, try a default system font or a font included with SFML
        if (!fontLoaded) {
            std::cout << "Warning: Could not load font. UI text may not display correctly." << std::endl;
            std::cout << "Algorithm details will be displayed in console." << std::endl;
        }
    }

    // SORTING ALGORITHM SET KARNE KA TAREEQA!

    void setSorter(SorterAlgorithm* newSorter) {
        stopSorting();
        if (sorter != nullptr) {
            delete sorter;
        }

        sorter = newSorter;

        // Print algorithm info to console

        if (sorter != nullptr) {
            cout << "\n---------------------------------" << endl;
            cout << "Algorithm: " << sorter->getAlgoName() << endl;
            cout << "Time Complexity: " << sorter->getT_C() << endl;
            cout << "Space Complexity: " << sorter->getS_C() << endl;
            cout << "---------------------------------\n" << endl;

            //Setting the call back function takay run time updates ho jaien 
            
            sorter->setCallback(updateHighlightsCallback);
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

         
            sortingThread = std::thread([this]() {
                sorter->sort();
                sorting = false;
                });

            sortingThread.detach();
        }
    }

     // Is ke through na sirf ham sorting ko band kar rahe hain, balke array ki values ko bhi reset kar rahe hain 
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

        highlight1 = global_highlight1; 
        highlight2 = global_highlight2; 

    }

    void draw() {
        window.clear(Color(30, 30, 30)); // Dark background
        array.draw(window, highlight1, highlight2, &font, fontLoaded);

        
        drawUIText("ALGOVIZ - SORTING VISUALIZER", 10, 10, 24);
        drawUIText("Press [S) to Start Sorting ||  (R) to Reset ||  (A) to Change Algorithm", 10, 40, 16);

        
        string algorithmText = "Current Algorithm: ";
        string complexityText = "Time Complexity: ";
        string spaceText = "Space Complexity: ";

        if (sorter != nullptr) {
            algorithmText += sorter->getAlgoName(); // Yahan par string concatenation ho rahi hai! 
            complexityText += sorter->getT_C();
            spaceText += sorter->getS_C();
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

    // Is method keh through ham log UI ko draw kar rahe hain. 
    void drawUIText(const std::string& text, float x, float y, unsigned int size) {
        if (fontLoaded) {
            

            sf::Text textObj(text, font, size);
            textObj.setPosition(x, y);
            textObj.setFillColor(sf::Color::White);
            window.draw(textObj);
        }
        else {
            sf::RectangleShape infoBar(sf::Vector2f(10, 10));
            infoBar.setFillColor(sf::Color::White);
            infoBar.setPosition(x, y);
            window.draw(infoBar);
        }
    }

    sf::RenderWindow& getWindow() {
        return window;
    }

    Array& getArray() {
        return array;
    }

    // FUNCTIONING FOR THE ALGORITHM CHOOSING MENU! 
    void chooseSortingAlgorithm() {
        // Is ke through ham log aik new window create kar rahe hain 
        sf::RenderWindow menuWindow(sf::VideoMode(600, 500), "Selecting Desired Algorithm ");

        
        if (!fontLoaded) {
            menuWindow.close();

            cout << "\n\n  HELLO USER! PLEASE CHOOSE YOUR DESIRED SORTING ALGORITHM \n";
            cout << "  --------------------------\n";
            cout << "  1. Bubble Sort - O(n²) - Compares adjacent elements and swaps if needed\n";
            cout << "  2. Insertion Sort - O(n²) - Builds sorted array one item at a time\n";
            cout << "  3. Selection Sort - O(n²) - Finds minimum and puts at beginning\n";
            cout << "\n  Choose algorithm (1-3): ";

            int choice;
            cin >> choice;

            //Utilizing swith case to allow the user to set the required type of the sorting algorithm. 

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
                //Default mein agar user utli option select karde tou bubble sort hi rehna chahiye! 
                setSorter(new BubbleSort(array));
                cout << "  Invalid choice! Defaulting to Bubble Sort.\n";
            }

            this_thread::sleep_for(std::chrono::milliseconds(1500));
            return;
        }

        // SFML text for menu items with more details
        sf::Text title("SORTING ALGORITHM SELECTION", font, 24);
        title.setPosition(50, 30);
        title.setFillColor(sf::Color::White);

        // Bubble Sort info
        sf::Text option1("1. Bubble Sort", font, 18);
        option1.setPosition(50, 100);
        option1.setFillColor(sf::Color::White);

        sf::Text bubble_complex("Time Complexity: O(n²) | Space Complexity: O(1)", font, 14);
        bubble_complex.setPosition(70, 125);
        bubble_complex.setFillColor(sf::Color::Yellow);

        sf::Text bubble_desc("Repeatedly compares adjacent elements and swaps them if in wrong order.", font, 12);
        bubble_desc.setPosition(70, 145);
        bubble_desc.setFillColor(sf::Color::Cyan);

        // Insertion Sort info
        sf::Text option2("2. Insertion Sort", font, 18);
        option2.setPosition(50, 180);
        option2.setFillColor(sf::Color::White);

        sf::Text insert_complex("Time Complexity: O(n²) | Space Complexity: O(1)", font, 14);
        insert_complex.setPosition(70, 205);
        insert_complex.setFillColor(sf::Color::Yellow);

        sf::Text insert_desc("Builds sorted array one item at a time. Efficient for small data sets.", font, 12);
        insert_desc.setPosition(70, 225);
        insert_desc.setFillColor(sf::Color::Cyan);

        // Selection Sort info
        sf::Text option3("3. Selection Sort", font, 18);
        option3.setPosition(50, 260);
        option3.setFillColor(sf::Color::White);

        sf::Text select_complex("Time Complexity: O(n²) | Space Complexity: O(1)", font, 14);
        select_complex.setPosition(70, 285);
        select_complex.setFillColor(sf::Color::Yellow);

        sf::Text select_desc("Finds minimum element and places it at beginning of unsorted array.", font, 12);
        select_desc.setPosition(70, 305);
        select_desc.setFillColor(sf::Color::Cyan);

        sf::Text instruction("Press 1, 2, or 3 to select | ESC to cancel", font, 16);
        instruction.setPosition(50, 350);
        instruction.setFillColor(sf::Color::Green);

        // Is ke through ham log menu ko control kar rahe hain! 
        while (menuWindow.isOpen()) {
            sf::Event event;
            while (menuWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    menuWindow.close();
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1) {
                        setSorter(new BubbleSort(array));
                        menuWindow.close();
                    }
                    else if (event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2) {
                        setSorter(new InsertionSort(array));
                        menuWindow.close();
                    }
                    else if (event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3) {
                        setSorter(new SelectionSort(array));
                        menuWindow.close();
                    }
                    else if (event.key.code == sf::Keyboard::Escape) {
                        menuWindow.close();
                    }
                }
            }

            //USING SFML TO DRAW THE BASIC STUFF OUT THERE 
            
            menuWindow.clear(Color(30, 30, 30));
            menuWindow.draw(title);

            // Draw Bubble Sort info
            menuWindow.draw(option1);
            menuWindow.draw(bubble_complex);
            menuWindow.draw(bubble_desc);

            // Draw Insertion Sort info
            menuWindow.draw(option2);
            menuWindow.draw(insert_complex);
            menuWindow.draw(insert_desc);

            // Draw Selection Sort info
            menuWindow.draw(option3);
            menuWindow.draw(select_complex);
            menuWindow.draw(select_desc);

            menuWindow.draw(instruction);
            menuWindow.display();
        }
    }

    // Clean up
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


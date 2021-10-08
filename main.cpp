#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>

std::vector<std::vector<int>> matrix;

bool* visited;
bool dfs(int N, int p);
bool issimple = true;

int main(int argc, char** argv) {

    // program can be used with command line argument
    // this argument used for handling .dot source file path
    // if argument list is empty, program create temp.dot file and scan adjacency matrix from cin
    // after generating file will be removed

    std::string source = "";

    if(argc == 2) {

        source = argv[1];

    }

    std::ifstream file;

    if(source.length()) { // checking for file exists if program used with command line argument


        file.open(source);

        if(!file.is_open()) {

            std::cout << "File doesn\'t exist, terminating..." << std::endl;
            return 0;

        }

        std::cin.rdbuf(file.rdbuf()); // redirecting cin to read from file

    }

    if(!source.length()) { // if file not presented, print invite to input

        std::cout << "Enter adjacency matrix: " << std::endl;

    }

    std::vector<int> temp; // temp vector for row scanning
    std::string input = "";

    std::getline(std::cin, input); // scanning one row for matrix size calculation

    int num;
    std::stringstream ss(input);

    while(ss >> num)
        temp.push_back(num); // size of temp on this step - num of columns and rows in matrix

    matrix.push_back(temp);
    int matrix_size = temp.size();

    for(int i = 0; i < matrix_size - 1; i++) { // scanning all rows to matrix

        temp.clear();
        input = "";

        std::getline(std::cin, input);

        std::stringstream ss(input);

        while(ss >> num)
            temp.push_back(num);

        matrix.push_back(temp);

    }

    if(file) // closing file if exists
        file.close();

    std::string dot_file = "graph my_graph{\n"; // generating temp.dot file content

    for(int i = 0; i < (int)matrix.size(); i++) {

        for(int j = 0; j <= i; j++) {

            if(matrix[i][j]) {

                dot_file += "\t";
                dot_file += (i + 1 + '0');
                dot_file += " -- ";
                dot_file += (j + 1 + '0');
                dot_file += " [label=\"";
                dot_file += matrix[i][j] + '0';
                dot_file += "\",label=\"";
                dot_file += matrix[i][j] + '0';
                dot_file += "\"]\n";

            }
        }
    }


    dot_file += "}";

    std::fstream temp_file;

    temp_file.open("temp.dot", std::ios::out);
    temp_file << dot_file; // writing temp.dot to file
    temp_file.close();


    std::string rm_dcommand = "rm "; // command for deleting temp.dot file
    rm_dcommand += "temp.dot";

    source = "temp.dot";

    std::string jpeg_fname = source; // name of .jpeg file
    jpeg_fname += ".jpeg";

    std::string gen_command = "dot "; // command for generating .jpeg file
    gen_command += source;
    gen_command += " -Tjpeg > ";
    gen_command += jpeg_fname;

    std::string create_command = "touch "; // command for creating .jpeg file
    create_command += jpeg_fname;

    std::string rm_jcommand = "rm "; // command for removing .jpeg file
    rm_jcommand += jpeg_fname;

    system(create_command.c_str()); // creating jpeg temp image
    system(gen_command.c_str()); // generating jpeg temp image

    visited = (bool*)malloc(sizeof(bool) * matrix.size()); // allocating memory for bool array

    bool cycle_check = false;

    for(int k = 0; k < matrix.size(); k++) { // checking graph for cycles

        for(int i = 0; i < matrix.size(); i++) // cleaning array
            visited[i] = false;

        if(dfs(k, k) && issimple) {
            cycle_check = true;
            break;
        }
        else if(!issimple){
            cycle_check = false;
            break;
        }

    }

    if(cycle_check)
        std::cout << "Graph has simple cycle" << std::endl;
    else
        std::cout << "Graph doesn\'t have simple cycle" << std::endl;

    sf::Image jpeg;
    jpeg.loadFromFile(jpeg_fname); // loading temp image for getting dimensions

    sf::Texture ready_image;
    ready_image.loadFromFile(jpeg_fname); // loading texture for shape background

    sf::RenderWindow window(sf::VideoMode(jpeg.getSize().x, jpeg.getSize().y), "Graph"); // creating window

    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(jpeg.getSize().x, jpeg.getSize().y));
    shape.setTexture(&ready_image); // creating rectangle shape with texture background

    system(rm_jcommand.c_str()); // removing jpeg temp image

    if(rm_dcommand.length())
        system(rm_dcommand.c_str()); // if program used without command line argument, removing temp.dot file

    while(window.isOpen()) { // drawing window with SFML

        sf::Event event;

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed)
                window.close();

        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    free(visited);

    return 0;
}

bool dfs(int N, int p){ // deep first search

    visited[N] = true;

    int count = 0;

    for(int i = 0; i < matrix.size(); i++)
        count += matrix[N][i];

    if(count > 2){
        issimple = false;
        return false;
    }

    for(int i = 0; i < matrix.size(); i++) {

        if(matrix[N][i] && i != p && !visited[i])
            return dfs(i, N);

        else if(matrix[N][i] && i != p && visited[i])
            return true;

    }

    return false;
}


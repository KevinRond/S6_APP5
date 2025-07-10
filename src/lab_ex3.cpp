#include <iostream>
#include <fstream>
#include <cstring>

/*
Commandes pour donner des inputs outputs a ce programme
input.txt | ./lab_ex3 > output.txt
./lab_ex3 < input.txt > output.txt      (tu peux mettre ./ devant le input.txt et/ou output.txt, ca change rien)
./lab_ex3 ./test.txt > times_2.txt

Les 2 premieres methodes utilisent stdin
La derniere utilise le fichier comme entree

Pour la question 3.8:
Enchainement des données commande:
./lab_ex3 ./test.txt > times_2.txt && ./lab_ex3 ./times_2.txt > times_4.txt
sans fichier intermediaire:
cat test.txt | ./lab_ex3 | ./lab_ex3 > times_4.txt
*/

int main(int argc, char** argv)
{
    std::ifstream file_in;

    if (argc >= 2 && (strcmp(argv[1], "-") != 0)) {
        file_in.open(argv[1]);
        if (file_in.is_open()) {
            std::cin.rdbuf(file_in.rdbuf());
            std::cerr << "Using " << argv[1] << "..." << std::endl;
        } else {
            std::cerr   << "Error: Cannot open '"
                        << argv[1] 
                        << "', using stdin (press CTRL-D for EOF)." 
                        << std::endl;
        }
    } else {
        std::cerr << "Using stdin (press CTRL-D for EOF)." << std::endl;
    }

    while (!std::cin.eof()) {

        std::string line;

        std::getline(std::cin, line);
        if (!line.empty()) {
            int val = atoi(line.c_str());
            std::cout << val * 2 << std::endl;
        }
    }

    if (file_in.is_open()) {
        file_in.close();
    }

    return 0;
}

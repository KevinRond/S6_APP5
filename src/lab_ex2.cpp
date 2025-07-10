#include <cstdio>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <cstdlib>      // rand

namespace {
    std::queue<int> queue_;
    std::mutex      mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_requested{false};
}

void add_to_queue(int v)
{
    // Fournit un accès synchronisé à queue_ pour l'ajout de valeurs.
    
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(v);
    cv_.notify_one();
}

void prod()
{
    // Produit 100 nombres aléatoires de 1000 à 2000 et les ajoute
    // à une file d'attente (queue_) pour traitement.
    // À la fin, transmet "0", ce qui indique que le travail est terminé.

    for (int i = 0; i < 100; ++i)
    {
        if (stop_requested.load()){
            printf("PROD: stop requested, ciao! D:\n");
            break;
        }
        int r = rand() % 1001 + 1000;
        add_to_queue(r);

        // Bloque le fil pour 50 ms:
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    add_to_queue(0);
}

void cons()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        // On doit toujours vérifier si un objet std::queue n'est pas vide
        // avant de retirer un élément.
        cv_.wait(lock, [] { return !queue_.empty() || stop_requested; });
        if (stop_requested.load()){
            printf("CONS: stop requested, ciao! D:\n");
            break;
        }

        int v = queue_.front(); // Copie le premier élément de la queue.
        queue_.pop();           // Retire le premier élément.

        printf("Reçu: %d\n", v);

        if (v == 0){
            printf("0 recu, bye bye! :)\n");
            break;
        }
    }

}

int main(int argc, char** argv)
{
    std::thread t_prod(prod);
    std::thread t_cons(cons);
    
    // Simulation d'une interruption
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_requested.store(true);  // ou stop_requested = true;
    } // Les brackets servent à dire au compilateur ou on peut unlock le mutex
    cv_.notify_all(); 
    // Fin de la simulation d'une interruption

    t_prod.join();
    t_cons.join();


    return 0;
}


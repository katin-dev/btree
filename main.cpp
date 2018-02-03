#include <stdio.h>

class BTreeNode {
    private:
        int t;
        bool isLeaf;
        int * keys;
        int length;
    public: 
        BTreeNode(int _t, bool _isLeaf) {
            this->t = _t;
            this->isLeaf = _isLeaf;
            this->length = 0;
            this->keys = new int[_t];
        }

        void append(int key) {
            if (this->isFull()) {
                // сложный алгоритм деления ноды
            } else {
                // добавляем новый ключ в массив ключей
                // идём справа налево, сдвигая вправо большие ключи                
                int i; 
                for (i = this->length - 1; i >= 0; i--) {
                    if (this->keys[i] > key) {
                        // Смещение элемента вправо
                        this->keys[i + 1] = this->keys[i];
                    } else {
                        // Нашли позицию, куда можно вставить новый элемент
                        break;
                    }
                }

                // Вставляем новый элемент на его место
                this->keys[i + 1] = key;

                // у нас на 1 элемент стало больше
                this->length ++;
                this->print();
            }
        }

        bool isFull() {
            return this->length == 2 * this->t - 1;
        }

        void print() {
            for(int i = 0; i < this->length; i++) {
                printf("%d ", this->keys[i]);
            }
            printf("\n");
        }
};

class BTree {
    private:
        int t = 1;
        BTreeNode * rootNode;

        // Создание корневого элемента
        BTreeNode * createRootNode() {
            BTreeNode * node = new BTreeNode(this->t, true);
            return node;
        }

    public: 
        BTree(int _t) {
            this->t = _t;
            this->rootNode = NULL;
        };

        void append(int key) {
            if (this->rootNode == NULL) {
                this->rootNode = this->createRootNode();
            }

            this->rootNode->append(key);
        }

        void print() {
            if (this->rootNode) {
                this->rootNode->print();
            }
        }
};


int main() {
    
    int t = 3;  // свойство дерева
    int keys[10] = {90, 30, 60, 10, 15, 80, 70, 20, 25, 5};

    BTree * tree = new BTree(t);

    // Наполняем дерево значениями
    for (int i = 0; i < 10; i++) {
        tree->append(keys[i]);
    }

    tree->print();

    return 0;
}
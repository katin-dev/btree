#include <time.h>
#include <stdio.h>
#include <stdlib.h>

class BTreeNode {
    private:
        int t;
        bool isLeaf;
        int * keys;
        int length;
        BTreeNode ** children;        
        BTreeNode * parent;
    public: 
        BTreeNode(int _t, bool _isLeaf, BTreeNode * _parent) {
            this->t = _t;
            this->isLeaf = _isLeaf;
            this->length = 0;
            this->keys = new int[2 * t - 1];
            this->children = new BTreeNode * [2 * t];
            this->parent = _parent;
            for (int i = 0; i < 2 * t; i++) {
                this->children[i] = NULL;
            }
        }

        bool isLeafNode() {
            bool isLeaf = true;
            for (int i = 0; i <= this->length; i++) {
                if (this->children[i] != NULL) {
                    isLeaf = false;
                    break;
                }
            }
            return isLeaf;
        }

        void split() {

            if (this->parent == NULL) {
                // printf("Split root node\n");
                // сложный алгоритм деления root node                
                BTreeNode * left  = new BTreeNode(this->t, true, this);
                BTreeNode * right = new BTreeNode(this->t, true, this);

                // раскидываем свои элементы по новым потомкам
                for (int i = 0; i < this->length; i ++) {
                    // printf("%d -> ", this->keys[i]);
                    if (i + 1 < this->t) {
                        // printf("left\n");
                        left->append(this->keys[i]);
                    } else if (i + 1 > this->t) {
                        // printf("right\n");
                        right->append(this->keys[i]);
                    } else {
                        // printf("_\n");
                    }
                }

                // копируем детей
                for (int j = 0; j <= this->length; j++) {
                    if (j + 1 < this->t) {
                        left->children[j] = this->children[j];
                    } else if (j + 1 > this->t) {
                        right->children[j - t] = this->children[j];
                    }
                }

                // printf("Got left node\n");
                left->print();
                // printf("\n");

                // printf("Got right node\n");
                right->print();
                // printf("\n");


                // у себя оставялем только один срединный элемент
                this->keys[0] = this->keys[this->t - 1];
                this->length = 1;

                this->children[0] = left;
                this->children[1] = right;



                this->isLeaf = false;                
            } else {
                // printf("Create new right sibling\n");
                // Создаю своего соседа сам
                BTreeNode * right  = new BTreeNode(this->t, true, this->parent);

                // отдаю ему все свои "правые" элементы
                for (int i = 0; i < this->length; i ++) {
                    if (i + 1 > this->t) {
                        // printf("%d => right node\n", this->keys[i]);
                        right->append(this->keys[i]);
                    }
                }

                int middle = this->keys[t - 1];
                // printf("%d => parent node\n", middle);

                // копируем детей в правую ноду
                // printf("Copy children...\n");
                for (int i = this->length; i >= this->t; i-- ) {
                    // printf("%d => %d\n", i, i - this->t);
                    right->children[i - this->t] = this->children[i];
                }


                this->length = t - 1;

                // Родителю надо передать новый middle и соответсвующий ему right node
                this->parent->forceAppend(middle, this, right);
            }
        }

        void append(int key) {

            if (this->isFull()) {
                this->split();

                if (this->parent) {
                    this->parent->append(key);
                } else {
                    this->append(key);
                }
            } else {

                // добавляем новый ключ в массив ключей
                // идём справа налево, сдвигая вправо большие ключи                
                if (this->isLeaf)  {
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
                    // this->print();
                } else {
                    // У нас есть дочерние элементы
                    int i; 
                    for (i = this->length - 1; i >= 0; i--) {
                        if (this->keys[i] < key) {
                            // нашли подходящий i
                            break;
                        }
                    }

                    // printf("Append to my child #%d\n", i + 1);
                    this->children[i + 1]->append(key);
                }
            }
        }

        void forceAppend(int key, BTreeNode * left, BTreeNode * right) {
            int i = this->length - 1;
            // printf("Return %d to parent\n", key);
            while (i >= 0 && this->keys[i] > key) {
                // сдвигаем ключи
                this->keys[i + 1] = this->keys[i];
                if (!this->isLeaf) {
                    // сдвигаем детей
                    this->children[i + 2] = this->children[i + 1];
                }
                i--;
            }

            // printf("Put into %d pos\n", i + 1);
            // printf("Left node to %d\n", i + 1);
            // printf("Right node to %d\n", i + 2);

            this->keys[i + 1] = key;
            this->children[i + 1] = left;
            this->children[i + 2] = right;
            this->length++;

            this->print();
            // printf("\n");
        }

        bool isFull() {
            return this->length == 2 * this->t - 1;
        }

        void print() {
            printf("[");
            int i;
            for(i = 0; i < this->length; i++) {
                if (!this->isLeafNode() && this->children[i] != NULL) {
                    this->children[i]->print();
                }
                printf("%d ", this->keys[i]);
            }
            if (!this->isLeaf) {
                this->children[i]->print();
            }
            printf("]");
        }
};

class BTree {
    private:
        int t = 1;
        BTreeNode * rootNode;

        // Создание корневого элемента
        BTreeNode * createRootNode() {
            BTreeNode * node = new BTreeNode(this->t, true, NULL);
            return node;
        }

    public: 
        BTree(int _t) {
            this->t = _t;
            this->rootNode = NULL;
        };

        void append(int key) {
            // printf("%d ==> ", key);
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
    // int keys[25] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};  

    BTree * tree = new BTree(t);

    int key = 0;
    /* while(scanf("%d", &key)) {
        printf("%d", key);
        tree->append(key);
        tree->print();
        printf("\n");
    } */

    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        int key = (int) rand() % 500;
        tree->append(key);
        tree->print();
        printf("\n");
    }

    return 0;
}
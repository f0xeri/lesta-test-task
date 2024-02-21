# Lesta Games test task

### Task 1

<details>
  <summary>Вопрос 1</summary>
    На языке С/С++, написать алгоритм (функцию) определения четности целого числа, который будет аналогичен нижеприведенному по функциональности, но отличен по своей сути.

Объяснить плюсы и минусы обеих реализаций.

C/C++ example:

bool isEven(int value){return value%2==0;}
</details>

```cpp
bool isEven2(int value){return (value & 1) == 0;}
```

С точки зрения читабельности первый вариант (value % 2) я считаю более предпочтительным. Взятие остатка от деления - 
очевидное действие, в то время как побитовое И заставит задуматься человека, который не знаком с этим "трюком".
С точки зрения производительности - при включенной оптимизации компиляторы показывают идентичные результаты для
обеих функций (по крайней мере на x86). Без оптимизаций предпочтительнее вариант с побитовым И.
https://godbolt.org/z/9PPfdf9fc

GCC 13.2 x86-64
```asm
mov     eax, edi
xor     eax, 1
and     eax, 1
ret
```
Clang 17.0.1 x86-64
```asm
test    dil, 1
sete    al
ret
```

Более того, компиляторы любят оптимизировать вызовы подобных функций даже без constexpr 

### Task 2

<details>
  <summary>Вопрос 2</summary>
На языке С++, написать минимум по 2 класса реализовывающих циклический буфер.

Объяснить плюсы и минусы каждой реализации.
</details>

Приведены две реализации:

RingBuffer.hpp - реализация на основе std::vector
RingBuffer2.hpp - реализация на основе std::list

В задании не указаны конкретные требования к реализуемым контейнерам. Реализованные контейнеры являются нерасширяемыми
циклическими буфферами с возможностью двусторонней итерации. Выбор нужной реализации сильно зависит от входных условий.
Реализация на std::vector аллоцирует память под все элементы сразу же при инициализации, таким образом 
операции push_back/pop_front не выполняют аллокации/деаллокации памяти, а лишь перемещают "указатели" на начало/конец.
Это может быть полезно при небольших размерах буфера и хранимых данных, а также частых операциях вставки/удаления.
Но если размер аллоцированной памяти играет значение, мы не так часто вставляем/удаляем элементы, то нам может подойти
вариант с двусвязным списком. В целом обе реализации можно улучшить по необходимости, например - реализовав random access,
в котором вариант со связным списком значительно проиграет.


### Task 3

<details>
  <summary>Вопрос 3</summary>
На языке С/С++, написать функцию, которая быстрее всего (по процессорным тикам) отсортирует данный ей массив чисел.

Массив может быть любого размера со случайным порядком чисел (в том числе и отсортированным).

Объяснить почему вы считаете, что функция соответствует заданным критериям.
</details>

Выбор нужного алогоритма сортировки значительно зависит от множества параметров - что мы сортируем, где оно хранится, 
какого размера, есть ли в массиве уже отсортированные элементы, можем ли мы использовать дополнительную память, 
требуется ли стабильность и так далее. 
Я выбрал классический алгоритм - quick sort.
Его недостатком является худший сценарий - полностью отсортированный массив "сортируется" за O(n^2), 
однако и эту проблему при необходимости можно избежать с помощью рандомизации pivot и/или рандомизации входных данных (не используется в приведенном коде)
Однако по всем остальным характеристикам для нашего сценария является одним из лучших.
При этом если требуется обобщенная функция, способная сортировать разные данные разных размеров, можно использовать алгоритмы, такие как introsort или pdqsort, которые "подстраиваются" под входные данные во время работы
и выбирают подходящий алгоритм сортировки. Подобные алгоритмы применяются в большинстве стандаратных библиотек.

Quicksort
```cpp
void quicksort(std::vector<int> &vec, int left, int right) {
    if (left < right) {
        int pivot = left;
        for (int i = left + 1; i <= right; ++i) {
            if (vec[i] < vec[left]) {
                std::swap(vec[++pivot], vec[i]);
            }
        }
        std::swap(vec[left], vec[pivot]);
        quicksort(vec, left, pivot - 1);
        quicksort(vec, pivot + 1, right);
    }
}
```

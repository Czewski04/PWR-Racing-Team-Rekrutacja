#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

class point{        //deklaracja klasy point - to na niej opera się caly program
public:             //klasa zawiera dwie zmienne odopowiadające współrzędnym punktów
    double x;
    double y;
};

void searching_for_the_first_vertices(std::vector<point> &apex, std::vector<point> &points, point &p1);
void searching_another_apexes(std::vector<point> &apex, std::vector<point> &points, point &p1);
void organizing_vecotrs(std::vector<point> &apex, std::vector<point> &points);
void validation(std::vector<point> &apex, std::vector<point> &points);
void removing_invalid_apexes(std::vector<point> &apex);

int main() {
    point p1;
    std::vector<point> apex;                           //deklaracja vecotrów apex oraz points składających się z obiektów klasy point, aby zapewnić elastyczność w operacjach na punktach
    std::vector<point> points;
    int how_much_numbers;
    std::fstream file_coordinates;

    std::string name_of_file;
    std::cout<<"Podaj nazwe pliku:\n";
    std::cin>>name_of_file;

    file_coordinates.open(name_of_file, std::ios::in);
    if(!file_coordinates.good())                     //sprawdzenie poprawności otwarcia pliku i ewnetualna informacja o błędzie
    {
        std::cout<<"Blad! - Nie otwarto pliku";
    }

    file_coordinates >> how_much_numbers;
    for (int i=0; i<how_much_numbers; i++)          //wczytanie liczb do pliku
    {
        points.push_back(p1);
        file_coordinates >> points[i].x;
        file_coordinates >> points[i].y;
    }

    searching_for_the_first_vertices(apex, points, p1);         //znajdowanie najbardziej wysuniętych wierzchołków po każdej stronie osi
    validation(apex, points);                                      //usuwanie powtórzynch wierzchołków, jeśli przykładowo któryś punkt ma największą wartość na osi X i na osi Y
    organizing_vecotrs(apex, points);                              //usuwanie wierzchołków z vectora punktów, jeśli dany punkt znajduje się w vectorze wierzchołków
    searching_another_apexes(apex, points, p1);                 //znajdowanie pozostałych wierzchołków algorytmem (algorytm wyjaśniony w dołącznym dokumencie)
    removing_invalid_apexes(apex);                                    //usuwanie wierzchołków które tworzą kąty wklęsłe (ostateczne sprawdzenie)

    std::cout<<"[";                                //wypisanie danych wyjściowych na ekran konsoli
    for (int i=0; i<apex.size();i++)
    {
        if(i==apex.size()-1)
            std::cout<<"("<<apex[i].x<<", "<<apex[i].y<<")]";
        else
            std::cout<<"("<<apex[i].x<<", "<<apex[i].y<<"), ";
    }
    return 0;
}

void searching_for_the_first_vertices(std::vector<point> &apex, std::vector<point> &points, point &p1)
{
    double tmp_x_min=1.79769e+308;               //nadanie wartości początkowych zmiennym, tak aby było możliwe sprawdzenie nawiększych i najmniejszych wartości na poszczegolnych osiach
    double tmp_y_min=1.79769e+308;
    double tmp_x_max=2.22507e-308;
    double tmp_y_max=2.22507e-308;
    for(int i=0; i<points.size(); i++)          //wyszukiwanie wartości na osiach
    {
        if(points[i].x<tmp_x_min)
        {
            tmp_x_min = points[i].x;
            if(!apex.empty())
                apex.pop_back();
            apex.push_back(p1);
            apex[0].x = points[i].x;
            apex[0].y = points[i].y;
        }
    }
    for(int i=0; i<points.size(); i++)
    {
        if(points[i].y<tmp_y_min)
        {
            tmp_y_min = points[i].y;
            if(apex.size()==2)
                apex.pop_back();
            apex.push_back(p1);
            apex[1].x = points[i].x;
            apex[1].y = points[i].y;
        }
    }
    for(int i=0; i<points.size(); i++)
    {
        if(points[i].x>tmp_x_max)
        {
            tmp_x_max = points[i].x;
            if(apex.size()==3)
                apex.pop_back();
            apex.push_back(p1);
            apex[2].x = points[i].x;
            apex[2].y = points[i].y;
        }
    }
    for(int i=0; i<points.size(); i++)
    {
        if(points[i].y>tmp_y_max)
        {
            tmp_y_max = points[i].y;
            if(apex.size()==4)
                apex.pop_back();
            apex.push_back(p1);
            apex[3].x = points[i].x;
            apex[3].y = points[i].y;
        }
    }
}

void organizing_vecotrs(std::vector<point> &apex, std::vector<point> &points)
{
    for(int i=0; i<apex.size(); i++)
    {
        for(int j=0; j<points.size(); j++)
        {
            if (apex[i].x == points[j].x && apex[i].y == points[j].y)
            {
                points.erase(points.begin()+j);
            }
        }
    }
}

void validation(std::vector<point> &apex, std::vector<point> &points)
{
    int tmp=0;
    for(int i=0; i<apex.size(); i++)            //usuwanie elementów z vectora points, jeśli element jest zawarty w vectorze apex
    {
        for(int j=i+1; j<apex.size(); j++)
        {
            if(apex[i].x == apex[j].x && apex[i].y == apex[j].y)
                apex.erase(apex.begin()+j);
        }
    }
    while(apex.size()<3)                            //pętla działąjąca gdy nie można znaleźć więcej niż 2 wierzchołków z najwyższymi bądź najniższymi wartościami na osiach
    {
        double wektor_apex_x, wektor_apex_y, wektor_point_x, wektor_point_y;

        wektor_point_x = points[tmp].x-apex[0].x;
        wektor_point_y = points[tmp].y-apex[0].y;
        wektor_apex_x = apex[1].x-apex[0].x;
        wektor_apex_y = apex[1].y-apex[0].y;
        if(wektor_apex_x*wektor_point_y-wektor_point_x*wektor_apex_y>0)          //sprawdzany jest warunek czy punkt znajduje się po lewej stronie lini utworzonej z dwóch skrajnych wierzchołków, aby zapewnić prawidłowe działanie programu
        {
            apex.push_back(points[tmp]);
            points.erase(points.begin()+tmp);
        }
        tmp+=1;
    }
}

void searching_another_apexes(std::vector<point> &apex, std::vector<point> &points, point &p1)
{
    double wektor_apex_x, wektor_apex_y, wektor_point_x, wektor_point_y;
    for(int i=0; i<points.size(); i++)
    {
        for(int j=1; j<apex.size(); j++)                 //znajdowanie innych wierzchołków wielokąta otaczającego inne punkty korzystając z właściwości iloczynu wektorowego
        {
            wektor_point_x = points[i].x-apex[0].x;
            wektor_point_y = points[i].y-apex[0].y;
            wektor_apex_x = apex[j].x-apex[0].x;
            wektor_apex_y = apex[j].y-apex[0].y;

            if(wektor_apex_x*wektor_point_y-wektor_point_x*wektor_apex_y<=0 && j==1)
            {
                apex.insert(apex.begin()+j,p1);
                apex[j].x = points[i].x;
                apex[j].y = points[i].y;
                removing_invalid_apexes(apex);
                break;
            }
            if(wektor_apex_x*wektor_point_y-wektor_point_x*wektor_apex_y>0 && j==apex.size()-1)
            {
                apex.push_back(p1);
                apex[apex.size()-1].x = points[i].x;
                apex[apex.size()-1].y = points[i].y;
                removing_invalid_apexes(apex);
                break;
            }
            if(wektor_apex_x*wektor_point_y-wektor_point_x*wektor_apex_y<=0)
            {
                wektor_point_x = points[i].x-apex[j-1].x;
                wektor_point_y = points[i].y-apex[j-1].y;
                wektor_apex_x = apex[j].x-apex[j-1].x;
                wektor_apex_y = apex[j].y-apex[j-1].y;

                if(wektor_apex_x*wektor_point_y-wektor_point_x*wektor_apex_y<=0)
                {
                    apex.insert(apex.begin()+j,p1);
                    apex[j].x = points[i].x;
                    apex[j].y = points[i].y;
                    removing_invalid_apexes(apex);
                    break;
                }
            }
        }
        removing_invalid_apexes(apex);
    }
}

void removing_invalid_apexes(std::vector<point> &apex)
{
    double wektor_apex_x_a, wektor_apex_y_a, wektor_apex_x_b, wektor_apex_y_b, vector_product, a_module, b_module, sinus;
    for(int i=1; i<apex.size()-1; i++)
    {
        if(i<apex.size()-1)                            //sprawdzanie czy któreś wierzchołki nie tworzą kątów wklęsłych w wielokącie przy użyciu iloczynu wektorowego i funkcji sinus oraz ewentualne usuwanie ich
        {
            wektor_apex_x_a = apex[i+1].x - apex[i].x;
            wektor_apex_y_a = apex[i+1].y - apex[i].y;
            wektor_apex_x_b = apex[i-1].x - apex[i].x;
            wektor_apex_y_b = apex[i-1].y - apex[i].y;
            vector_product = wektor_apex_x_a * wektor_apex_y_b - wektor_apex_y_a * wektor_apex_x_b;
            a_module = sqrt(pow(wektor_apex_x_a,2) + pow(wektor_apex_y_a,2));
            b_module = sqrt(pow(wektor_apex_x_b,2) + pow(wektor_apex_y_b,2));
            sinus = vector_product / (a_module * b_module);
            if(sinus < 0)
                apex.erase(apex.begin()+i);
        }
        else
        {
            wektor_apex_x_a = apex[0].x - apex[i].x;
            wektor_apex_y_a = apex[0].y - apex[i].y;
            wektor_apex_x_b = apex[i-1].x - apex[i].x;
            wektor_apex_y_b = apex[i-1].y - apex[i].y;
            vector_product = wektor_apex_x_a * wektor_apex_x_b + wektor_apex_y_a * wektor_apex_y_b;
            a_module = sqrt(pow(wektor_apex_x_a,2) + pow(wektor_apex_y_a,2));
            b_module = sqrt(pow(wektor_apex_x_b,2) + pow(wektor_apex_y_b,2));
            sinus = vector_product / (a_module * b_module);
            if(sinus < 0)
                apex.erase(apex.begin());
        }
    }
}

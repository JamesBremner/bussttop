#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <wex.h>
#include "cStarterGUI.h"
#include "cDataPoint.h"
#include "KMeans.h"

class cRoad
{
public:
    std::pair<int, int> myStart;
    std::pair<int, int> myEnd;

    cRoad(
        const std::pair<int, int> &start,
        const std::pair<int, int> &end)
        : myStart(start), myEnd(end)
    {
    }
};
class cStop
{
public:
    std::pair<int, int> myLoc;
    int myNeed;
    bool myfActual;

    cStop(const std::pair<int, int> &loc)
        : myLoc(loc), myfActual(false)
    {
    }
};
class cSolution
{
public:
    void generateRoads();
    void potentialStops();
    void CalculateNeed();
    void selectStops();

    void draw(wex::shapes &S);

private:
    std::vector<cRoad> myRoad;
    std::vector<cStop> myPotentialStops;
    int myCountActualStops;

    std::pair<int, int> panZoom(const std::pair<int, int> &loc);
    void clearSelected();
};

int dist2(
    const std::pair<int, int> &a,
    const std::pair<int, int> &b)
{
    int dx = a.first - b.first;
    int dy = a.second - b.second;
    return dx * dx + dy * dy;
}

void cSolution::generateRoads()
{
    for (int k = 0; k < 5; k++)
    {
        myRoad.push_back(cRoad(
            {5 * k, 0}, {5 * k, 25}));
        myRoad.push_back(cRoad(
            {0, 5 * k}, {25, 5 * k}));
    }
}
void cSolution::potentialStops()
{
    for (auto &r : myRoad)
    {
        auto loc = r.myStart;
        std::pair<int, int> deltaloc{
            (r.myEnd.first - r.myStart.first) / 5,
            (r.myEnd.second - r.myStart.second) / 5};
        loc.first += deltaloc.first / 2;
        loc.second += deltaloc.second / 2;
        for (int k = 0; k < 5; k++)
        {
            myPotentialStops.push_back(loc);
            loc.first += deltaloc.first;
            loc.second += deltaloc.second;
        }
    }

    myCountActualStops = 7;
}

void cSolution::CalculateNeed()
{
    srand(time(NULL));
    for (auto &s : myPotentialStops)
    {
        if (s.myLoc.first == 10)
            s.myNeed = rand() % 10 + 10;
        else
            s.myNeed = rand() % 2 + 1;
    }
}

std::pair<int, int> cSolution::panZoom(const std::pair<int, int> &loc)
{
    const int scale = 10;
    const int panx = 20;
    const int pany = 20;

    std::pair<int, int> ret;
    ret.first = panx + scale * loc.first;
    ret.second = pany + scale * loc.second;
    return ret;
}

void cSolution::draw(wex::shapes &S)
{
    int scale = 10;
    S.color(0xFF0000);
    for (auto &r : myRoad)
    {
        auto p1 = panZoom(r.myStart);
        auto p2 = panZoom(r.myEnd);
        S.line(
            {p1.first, p1.second,
             p2.first, p2.second});
    }

    for (auto &ps : myPotentialStops)
    {
        auto p1 = panZoom(ps.myLoc);
        S.color(0x00FF00);
        S.circle(
            p1.first, p1.second,
            4);
        S.color(0x000000);
        S.text(std::to_string(ps.myNeed),
               {p1.first, p1.second,
                p1.first + 50, p1.second + 30});
    }
    S.color(0x0000FF);
    S.fill(false);
    for (auto &ps : myPotentialStops)
    {
        if (!ps.myfActual)
            continue;
        auto p1 = panZoom(ps.myLoc);
        S.circle(
            p1.first, p1.second,
            5);
    }
}

void cSolution::selectStops()
{
    // Construct the KMeans class
    KMeans KM;

    // loop over potential bus stops
    for (auto &ps : myPotentialStops)
    {
        //Each unit of need at the potential bust stop
        // is represented by "a need" at the location
        for (int k = 0; k < ps.myNeed; k++)
        {
            cDataPoint l(2);
            l.d[0] = ps.myLoc.first;
            l.d[1] = ps.myLoc.second;
            KM.Add(l);
        }
    }

    // initialize KMeans with the number of actual bust stops reuired
    KM.Init( myCountActualStops, false );

    // run KMeans algorithm to find clusters of need
    for( int kiter=0; kiter < 10; kiter++ )
    {
        KM.Assign();
        KM.MoveClustersToMean();
    }

    // Select bus stops nearest to cluster centers
    for( auto& c : KM.clusters() )
    {
        float min = 1e10;
        int nearest;
        int ks = -1;
        for (auto &ps : myPotentialStops)
        {
            ks++;
            float td = dist2( 
                ps.myLoc,
                { c.center().d[0],  c.center().d[1]});
            if( td < min )
            {
                min = td;
                nearest = ks;
            }
        }
        // convert nearest potential bus stop to an actual bus stop.
        myPotentialStops[nearest].myfActual = true;
    }

}
void cSolution::clearSelected()
{
    for (auto &s : myPotentialStops)
        s.myfActual = false;
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Bus Stops",
              {50, 50, 1000, 500})
    {

        mySolution.generateRoads();
        mySolution.potentialStops();
        mySolution.CalculateNeed();
        mySolution.selectStops();

        fm.events().draw(
            [&](PAINTSTRUCT &ps)
            {
                wex::shapes S(ps);
                mySolution.draw(S);
            });

        show();
        run();
    }

private:
    cSolution mySolution;
};

main()
{

    cGUI theGUI;
    return 0;
}

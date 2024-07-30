#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <climits>

using namespace std;

#define mod 1000000007
#define val 1000000001
#define debug(x) cout<< #x <<"="<<x<<"\n";
#define in(x) cin>>x
#define all(x) x.begin(),x.end()
#define PI 3.1415926535897932384626

int t=1,tq;

vector<vector<int>> non_premenitive_priority(map<char,vector<int>>& m);

bool sortbyCond(const vector<int>& a,const vector<int>& b)
{
    if(a[1]!=b[1])
       return (a[1] < b[1]);
    else if (a[0] != b[0])
        return a[0]<b[0];
    else
        return (a[2]<b[2]);
}

bool sortbyCond1(const pair<int, int>& a,
                const pair<int, int>& b)
{
    if (a.first != b.first)
        return (a.first < b.first);
    else
       return (a.second > b.second);
}

vector<vector<int>> first_come_first_serve(map<char,vector<int>>& m)
{
    int n=m['a'].size();
    vector<pair<int,int>> ar(n);
    for(int i=0;i<n;i++)
    {
        ar[i].first=m['a'][i];
        ar[i].second=i;
    }
    sort(all(ar));
    vector<vector<int>> time(n,vector<int> (3));
    time[0][0]=ar[0].first;
    time[0][1]=ar[0].first+m['b'][ar[0].second];
    time[0][2]=ar[0].second;
    for(int i=1;i<n;i++)
    {
        time[i][0]=max(time[i-1][1],ar[i].first);
        time[i][1]=time[i][0]+m['b'][ar[i].second];
        time[i][2]=ar[i].second;
    }
    return time;

}

vector<vector<int>> shortestjob(map<char,vector<int>>& m)
{
    int n=m['a'].size();
    vector<vector<int>> ar(n, vector<int> (3));
    for(int i=0;i<n;i++)
    {
        ar[i][0]=m['a'][i];
        ar[i][1]=m['b'][i];
        ar[i][2]=i;
    }
    //0 = start point in time
    //1 = end point in time
    //2 = index in m
    sort(all(ar));
    vector<vector<int>> time(n,vector<int> (3));
    time[0][0]=ar[0][0];
    time[0][1]=ar[0][0]+m['b'][ar[0][2]];
    time[0][2]=ar[0][2];
    vector<pair<int,int>> in;
    int ti=0;
    for(int i=1;i<n;i++)
    {
        if(time[ti][1]>=ar[i][0])
        {
            in.push_back({ar[i][1],ar[i][2]});
        }else{
            sort(all(in));
            for(int j=0;j<(int)in.size();j++)
            {
                ++ti;
                time[ti][0]=max(time[ti-1][1],m['a'][in[j].second]);
                time[ti][1]=time[ti][0]+m['b'][in[j].second];
                time[ti][2]=in[j].second;
            }
            ++ti;
            time[ti][0]=max(time[ti-1][1],ar[i][0]);
            time[ti][1]=time[ti][0]+m['b'][ar[i][2]];
            time[ti][2]=ar[i][2];
            in.clear();
        }
    }
    sort(all(in));
    for(int j=0;j<(int)in.size();j++)
    {
        ++ti;
        time[ti][0]=max(time[ti-1][1],m['a'][in[j].second]);
        time[ti][1]=time[ti][0]+m['b'][in[j].second];
        time[ti][2]=in[j].second;
    }
    return time;

}

vector<vector<int>> round_robin(map<char,vector<int>>& m)
{
    int n=m['a'].size();
    vector<vector<int>> ar(n, vector<int> (3));
    for(int i=0;i<n;i++)
    {
        ar[i][0]=m['a'][i];
        ar[i][1]=m['b'][i];
        ar[i][2]=i;
    }
//    0 == arrival
//    1 == buffer
//    2 == index
    sort(all(ar));
    vector<vector<int>> time(1,vector<int> (3));
    time[0][0]=ar[0][0];
    time[0][1]=time[0][0]+min(ar[0][1],m['t'][0]);
    ar[0][1]-=m['t'][0];
    time[0][2]=ar[0][2];
    vector<vector<int>> in;
    int ti=0,ai=1;
    in.push_back({ar[0][1],ar[0][0],ar[0][2]});
    while(ai<(int)ar.size() || (int)in.size()!=0)
    {
        while(ai<(int)ar.size() && ar[ai][0]<=time[ti][1])
        {
            in.push_back({ar[ai][1],ar[ai][0],ar[ai][2]}); //buffer arrival index
            ai++;
        }
        //sort(all(in),sortbyCond);
        if((int)in.size()==0)
        {
            continue;
        }
        if(in[0][0]<=0)
        {
            in.erase(in.begin());
            continue;
        }
        if((int)in.size()==0)
        {
            continue;
        }

        int ij=-1;
        for(int i=0;i<(int)in.size();i++)
        {
            if(in[i][2]==time[ti][2] && (int)in.size()!=1)
            {
                in.push_back(in[i]);
                ij=i;
            }else{
                break;
            }
        }
        if(ij!=-1)
        {
            in.erase(in.begin(),in.begin()+ij+1);
        }

        vector<int> tem(3);

        ++ti;
        tem[0]=max(time[ti-1][1],in[0][1]);
        tem[1]=tem[0]+min(in[0][0],m['t'][0]);
        in[0][0]-=m['t'][0];
        tem[2]=in[0][2];
        time.push_back(tem);

    }

    return time;

}
/*
5
6
0 1 2 3 4 6
4 5 2 1 6 3
2
*/

vector<vector<int>> shortestremainingtime(map<char,vector<int>>& m)
{
    int n=m['a'].size();
    vector<int> wt(n),ct(n),tat(n),rt(n),check(n,0),start(n,-1),rest(n);
    vector<vector<int>> ti;
    for(int i=0; i<n; i++)
    {
        rest[i]=m['b'][i];
    }

    int comt=0;
    int mini=INT_MAX;
    for(int i=0;i<n;i++)
    {
        mini=min(m['a'][i],mini);
    }
    int burst=mini;
    for(int i=0;i<n;i++)
    {
        burst+=m['b'][i];
    }
    int maxi=INT_MIN;
    for(int i=0;i<n;i++)
    {
        if(m['a'][i]>maxi)
        {
            maxi=m['a'][i];
        }
    }
    burst+=maxi;
    int coun1=n;
    queue<int> que1;
    vector<int> que2(n, 0);
    comt=mini;
    while(burst--)
    {
        if(coun1==0)
        {
            break;
        }
        for(int i=0;i<n;i++)
        {
            if(m['a'][i]<= comt && check[i]!=1 && que2[i]!=1)
            {
                que1.push(i);
                que2[i] = 1;
            }
        }
        int minib=INT_MAX;
        int minib_i;
        int p=que1.size();
        for(int i=0;i<p;i++)
        {
            int temp=que1.front();
            que1.pop();
            if(rest[temp]<minib)
            {
                minib=rest[temp];
                minib_i=temp;
            }
            que1.push(temp);
        }
        rest[minib_i]--;
        if(start[minib_i]==-1)
        {
            start[minib_i]=comt;
        }
        comt++;
        if(rest[minib_i]==0)
        {
            coun1--;
            for(int i=0;i<p;i++)
            {
                int x=que1.front();
                que1.pop();
                if(x!=minib_i)
                {
                    que1.push(x);
                }
            }
            check[minib_i]=1;
            ct[minib_i]=comt;
            tat[minib_i]=comt-m['a'][minib_i];
            wt[minib_i]=tat[minib_i]-m['b'][minib_i];
            rt[minib_i]=start[minib_i]-m['a'][minib_i];
        }
    }
    ti={wt,rt,tat,{comt},{mini}};
    return ti;
}

vector<vector<int>> priority(map<char,vector<int>>& m)
{
    int n=m['a'].size();
    vector<int> wt(n),ct(n),tat(n),rt(n),check(n, 0),start(n, -1),rest(n);
    vector<vector<int>> ti;
    for(int i=0;i<n;i++)
    {
        rest[i]=m['b'][i];
    }
    int mini=INT_MAX;
    for(int i=0;i<n;i++)
    {
        mini=min(mini,m['a'][i]);
    }
    int comt=mini;
    int coun1=n;
    queue<int> que1;
    vector<int> leave(coun1,0);
    while(coun1!=0)
    {
        for(int i=0;i<n;i++)
        {
            if(m['a'][i]<=comt && check[i]!= 1 && leave[i]!=1)
            {
                que1.push(i);
                leave[i] = 1;
            }
        }
        int pent=INT_MAX;
        int pentin;
        int p=que1.size();
        for(int i=0;i<p;i++)
        {
            int temp=que1.front();
            que1.pop();
            if(m['p'][temp]<pent)
            {
                pent = m['p'][temp];
                pentin = temp;
            }
            que1.push(temp);
        }
        rest[pentin]--;
        if(start[pentin]==-1)
        {
            start[pentin]=comt;
        }
        comt++;
        if(rest[pentin]==0)
        {
            coun1--;
            for(int i=0; i<p; i++)
            {
                int temp=que1.front();
                que1.pop();
                if(temp!= pentin)
                {
                    que1.push(temp);
                }
            }
            check[pentin]=1;
            ct[pentin]=comt;
            tat[pentin]=comt-m['a'][pentin];
            wt[pentin]=tat[pentin]-m['b'][pentin];
            rt[pentin]=start[pentin]-m['a'][pentin];
        }
    }
    ti={wt,rt,tat,{comt},{mini}};
    return ti;

}

void solution()
{
    int n;
    cin>>n;
    map<char,vector<int>> m;
    string s="abpt";
    for(int i=0;i<4;i++)
        m[s[i]]=vector<int> (n);
    for(int i=0;i<n;i++)
    {
        cin>>m['a'][i];
    }
    for(int i=0;i<n;i++)
    {
        cin>>m['b'][i];
    }
    if(t==4)
    {
        for(int i=0;i<n;i++)
        {
            cin>>m['p'][i];
        }
    }
    if(t==5)
    {
        cin>>tq;
        for(int i=0;i<n;i++)
        {
            m['t'][i]=tq;
        }
    }
    int comt,startt;
    vector<vector<int>> time,ti;
    vector<int> tat (n,0),wt(n,0),rt(n,0);
    bool b=true;
    switch(t)
    {
        case 1:
            time=first_come_first_serve(m);
            break;
        case 2:
            time=shortestjob(m);
            break;
        case 3:
            ti=shortestremainingtime(m);
            tat=ti[2];
            wt=ti[0];
            rt=ti[1];
            comt=ti[3][0];
            startt=ti[4][0];
            b=false;
            break;
        case 4:
            ti=priority(m);
            tat=ti[2];
            wt=ti[0];
            rt=ti[1];
            comt=ti[3][0];
            startt=ti[4][0];
            b=false;
            break;
        case 5:
            time=round_robin(m);
            break;
    }
    if(b)
    {
        vector<int> maxi(n,INT_MIN),mini(n,INT_MAX);
        for(int i=0;i<(int)time.size();i++)
        {
            maxi[time[i][2]]=max(maxi[time[i][2]],time[i][1]);
            mini[time[i][2]]=min(maxi[time[i][2]],time[i][0]);
        }
        for(int i=0;i<n;i++)
        {
            tat[i]=maxi[i]-m['a'][i];
        }
        for(int i=0;i<n;i++)
        {
            wt[i]=tat[i]-m['b'][i];
        }
        for(int i=0;i<n;i++)
        {
            rt[i]=mini[i]-m['a'][i];
        }
        comt=time[0][0];
        startt=time.back()[1];
    }

    for(int i=0;i<n;i++)
    {
        cout<<wt[i]<<" ";
    }
    cout<<"\n";
    for(int i=0;i<n;i++)
    {
        cout<<rt[i]<<" ";
    }
    cout<<"\n";
    for(int i=0;i<n;i++)
    {
        cout<<tat[i]<<" ";
    }
    cout<<"\n";
//    wt=tat-b;
//    tat=ct-ar;
//    rt=st-ar;
//    throughput=(n/(last time-starting time));
    cout<<setprecision(4)<<((double)n/(comt-startt))<<"\n";

}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin>>t;

    solution();

    return 0;
}

vector<vector<int>> non_premenitive_priority(map<char,vector<int>>& m)
{
    int n=m['a'].size();
    vector<vector<int>> ar(n, vector<int> (3));
    for(int i=0;i<n;i++)
    {
        ar[i][0]=m['a'][i];
        ar[i][1]=m['p'][i];
        ar[i][2]=i;
    }
    //0 = start point in time
    //1 = end point in time
    //2 = index in m
    sort(all(ar));
//    for(int i=0;i<n;i++)
//    {
//        for(int j=0;j<3;j++)
//        {
//            cout<<ar[i][j]<<" ";
//        }
//        cout<<"\n";
//    }
    vector<vector<int>> time(n,vector<int> (3));
    time[0][0]=ar[0][0];
    time[0][1]=time[0][0]+m['b'][ar[0][2]];
    time[0][2]=ar[0][2];
//    cout<<time[0][0]<<" "<<time[0][1]<<" "<<time[0][2]<<"\n";
    vector<pair<int,int>> in;
    int ti=0;
    for(int i=1;i<n;i++)
    {
        debug(time[ti][1]);
        debug(ar[i][0]);
        if(time[ti][1]>=ar[i][0])
        {
            in.push_back({ar[i][1],ar[i][2]});
        }else{
            sort(all(in));
            for(int j=0;j<(int)in.size();j++)
            {
                ++ti;
                time[ti][0]=max(time[ti-1][1],m['a'][in[j].second]);
                time[ti][1]=time[ti][0]+m['b'][in[j].second];
                time[ti][2]=in[j].second;
            }
            in.clear();
            ti++;
            time[ti][0]=max(time[ti-1][1],ar[i][0]);
            time[ti][1]=time[ti][0]+m['b'][ar[i][2]];
            time[ti][2]=ar[i][2];
        }
    }
    sort(all(in));
    for(int j=0;j<(int)in.size();j++)
    {
        ++ti;
        time[ti][0]=max(time[ti-1][1],m['a'][in[j].second]);
        time[ti][1]=time[ti][0]+m['b'][in[j].second];
        time[ti][2]=in[j].second;
    }
    return time;

}

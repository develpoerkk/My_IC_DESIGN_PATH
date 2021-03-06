#ifndef MAPSET_H
#define MAPSET_H
#include"map.h"
#include<iostream>
#include<sstream>
#include<string>


template<typename ValueType>
class MapSet
{
public:

    MapSet();
    ~MapSet();
    int size() const;
    bool isEmpty();
    void add(const ValueType & value);
    void remove(const ValueType & value);

    bool contains(const ValueType & value) const;
    void clear();

    bool isSubsetOf(const MapSet<ValueType> & set) const;

    bool operator!=(const MapSet<ValueType> & set) const;
    bool operator==(const MapSet<ValueType> & set) const;



    MapSet<ValueType> operator+(const MapSet<ValueType> & set2) const;
    MapSet<ValueType> operator+(const ValueType & value) const;
    MapSet<ValueType> operator*(const MapSet<ValueType> & set2) const;
    MapSet<ValueType> operator-(const MapSet<ValueType> & set2) const;
    MapSet<ValueType> operator-(const ValueType & value) const;

    MapSet<ValueType> & operator=(const MapSet<ValueType> & set2);

    MapSet<ValueType> & operator+=(const MapSet<ValueType> & set2);
    MapSet<ValueType> & operator+=(const ValueType & value);
    MapSet<ValueType> & operator*=(const MapSet<ValueType> & set2);
    MapSet<ValueType> & operator-=(const MapSet<ValueType> & set2);
    MapSet<ValueType> & operator-=(const ValueType & value);



    template<class T>
    friend std::ostream & operator<< (std::ostream & os, const MapSet<T> & set);
    template<class T>
    friend std::istream & operator>> (std::istream & is,  MapSet<T> & set);

    class iterator{
    public:
        iterator(){
            sp=NULL;
        }


        ValueType& operator*(){
            if(sp==NULL) error("Uninited iterator being used!");
            if(index<0||index>sp->map.size()) error("Iterator out of range!");
            typename Map<ValueType,bool>::iterator it=sp->map.begin();
            for(int i=0;i<index;i++) it++;
            return *it;
        }
        ValueType* operator->(){
            if(sp==NULL) error("Uninited iterator being used!");
            if(index<0||index>sp->map.size()) error("Iterator out of range!");
            typename Map<ValueType,bool>::iterator it=sp->map.begin();
            for(int i=0;i<index;i++) it++;
            return &it; // It's great to know what happend.
        }
        ValueType& operator[](int offset){
            if(sp==NULL) error("Uninited iterator being used!");
            if(offset+index<0||offset+index>sp->map.size()) error("Iterator out of range!");
            typename Map<ValueType,bool>::iterator it=sp->map.begin();
            for(int i=0;i<index+offset;i++) it++;
            return *it;
        }
        // realation operator start from here!
        bool operator==(const iterator& rhs){
            if(sp!=rhs.sp) error("Iterators are in different sets");
            return index==rhs.index;
        }
        bool operator!=(const iterator& rhs){
            if(sp!=rhs.sp) error("Iterators are in different sets");
            return index!=rhs.index;
        }
        bool operator <(const iterator& rhs){
            if(sp!=rhs.sp) error("Iterators are in different sets");
            return index<rhs.index;
        }
        bool operator >(const iterator& rhs){
            if(sp!=rhs.sp) error("Iterators are in different sets");
            return index>rhs.index;
        }
        bool operator <=(const iterator& rhs){
            if(sp!=rhs.sp) error("Iterators are in different sets");
            return index<=rhs.index;
        }
        bool operator >=(const iterator& rhs){
            if(sp!=rhs.sp) error("Iterators are in different sets");
            return index>=rhs.index;
        }
        // ++ and -- operators start from here

        iterator& operator++(){
            if(sp==NULL) error("Uninited iterator being used!");
            index++;
            return *this;
        }
        iterator operator++(int){
            iterator copy(*this);
            operator++();
            return copy;
        }
        iterator& operator--(){
            if(sp==NULL) error("Uninited iterator being used!");
            index--;
            return *this;
        }
        iterator operator--(int){
            iterator copy(*this);
            operator--();
            return copy;
        }
        // arithmetic operators start from here
        iterator operator+(const int &offset){
            if(sp==NULL) error("Uninited iterator being used!");
            return iterator(sp,offset+index); // Godd Design whitout wasting.
        }
        iterator operator-(const int &offset){
            return operator+((-1)*offset);
        }

        int operator-(const iterator& rhs){
            if(sp==NULL) error("Uninited iterator being used!");
            if(sp!=rhs.sp) error("Iterators are in different sets");
            return index-rhs.index;
        }


    private:
        MapSet* sp;
        int index;
        iterator(MapSet* sp,int index){
            this->sp=sp;
            this->index=index;
        }
        friend class MapSet;
    };

    iterator begin(){
        return iterator(this,0);
    }
    iterator end(){
        return iterator(this,map.size());
    }

private:

    Map<ValueType,bool> map;

};



template<typename ValueType>
MapSet<ValueType>::MapSet()
{

}

template<typename ValueType>
MapSet<ValueType>::~MapSet()
{

}

template<typename ValueType>
int MapSet<ValueType>::size() const
{
    return map.size();
}
template<typename ValueType>
bool MapSet<ValueType>::isEmpty()
{
    return map.size()==0;
}
template<typename ValueType>
void MapSet<ValueType>::add(const ValueType &value)
{
    map.add(value,true);

}
template<typename ValueType>
void MapSet<ValueType>::remove(const ValueType &value)
{
    map[value]=false;
    map.remove(value);
}
template<typename ValueType>
bool MapSet<ValueType>::contains(const ValueType &value) const
{
    return map.containsKey(value);
}
template<typename ValueType>
void MapSet<ValueType>::clear()
{
    map.clear();
}

template<typename ValueType>
bool MapSet<ValueType>::isSubsetOf(const MapSet<ValueType> &set) const
{
    for(ValueType value : set){
        if(!map.containsKey(value))
            return false;
    }
    return true;
}

template<typename ValueType>
bool MapSet<ValueType>::operator!=(const MapSet<ValueType> &set) const
{
    if(map.size()!=set.size()) return false;
    for(ValueType value : map)
        if(!set.contains(value)) return false;
    return true;
}

template<typename ValueType>
bool MapSet<ValueType>::operator==(const MapSet<ValueType> &set) const
{
    return !((*this)!=set);
}

template<typename ValueType>
MapSet<ValueType> MapSet<ValueType>::operator+(const MapSet<ValueType> &set2) const
{
    MapSet<ValueType> targetSet=*this;
    for(ValueType value : set2.map)
        targetSet.add(value);
    return targetSet;
}

template<typename ValueType>
MapSet<ValueType> MapSet<ValueType>::operator+(const ValueType &value) const
{
    MapSet<ValueType> targetSet=*this;
    targetSet.add(value);
    return targetSet;
}

template<typename ValueType>
MapSet<ValueType> MapSet<ValueType>::operator*(const MapSet<ValueType> &set2) const
{
    MapSet<ValueType> targetSet;
    for(ValueType value : map){
        if(set2.map.containsKey(value))
            targetSet.add(value);
    }
    return targetSet;
}

template<typename ValueType>
MapSet<ValueType> MapSet<ValueType>::operator-(const MapSet<ValueType> &set2) const
{
    MapSet<ValueType> targetSet=*this;
    for(ValueType value : set2.map)
        targetSet.remove(value);
    return targetSet;
}


template<typename ValueType>
MapSet<ValueType> MapSet<ValueType>::operator-(const ValueType &value) const
{
    MapSet<ValueType> targetSet=*this;
    targetSet.remove(value);
    return targetSet;
}

template<typename ValueType>
MapSet<ValueType> &MapSet<ValueType>::operator=(const MapSet<ValueType> &set2)
{
    for(ValueType value1 : (*this - set2).map )
        remove(value1);

    for(ValueType value2 : (set2 - *this).map )
        add(value2);
    return *this;
}
template<typename ValueType>
MapSet<ValueType> &MapSet<ValueType>::operator+=(const MapSet<ValueType> &set2)
{
    *this=(*this)+set2;
}
template<typename ValueType>
MapSet<ValueType> &MapSet<ValueType>::operator+=(const ValueType &value)
{
    *this=(*this)+value;
}
template<typename ValueType>
MapSet<ValueType> &MapSet<ValueType>::operator*=(const MapSet<ValueType> &set2)
{
    *this=(*this)*set2;
}
template<typename ValueType>
MapSet<ValueType> &MapSet<ValueType>::operator-=(const MapSet<ValueType> &set2)
{
    *this=(*this)-set2;
}
template<typename ValueType>
MapSet<ValueType> &MapSet<ValueType>::operator-=(const ValueType &value)
{
    remove(value);
    return *this;
}

template<typename ValueType>
std::ostream &operator<<(std::ostream & os, const MapSet<ValueType> & set)
{
    int size=set.size();
    int i=1;
    std::cout<<"{";
    for(ValueType value : set.map ){
        os<<value;
        if(i==size)
            os<<"}";
        else{
            os<<",";
            i++;
        }
    }
    if(set.size()==0) os<<"}";
    return os;
}

template<typename ValueType>
std::istream &operator>>(std::istream &is,  MapSet<ValueType> &set)
{
    std::string str="";
    if(is.get()!='{') {

        error("Invaild format! example: {a,b,c}");
    }
    std::stringstream ss;
    while(true){
        char word=is.get();

        if(word!=','&&word!='}'){
            ss<<word;
        }
        else{
            ss>>str;
            set.add(str);
            ss.clear();
            if(word=='}')
                break;
        }
    }
    is.get(); //The input stream will read change-line symbol
    return is;
}


#endif // MAPSET_H

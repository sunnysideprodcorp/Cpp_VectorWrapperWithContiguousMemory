#ifndef SMALL_VECTOR_H
#define SMALL_VECTOR_H

#include "VectorClasses.h"
#include "LoggingPolicy.h"

#include <cstddef>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include <string>

template <class T, std::size_t N, std::size_t MAX_SIZE, int ID, template <class> class MemPolicy>
  class SmallVector
{ 
  typedef  T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type& reference ;
  typedef const  value_type& const_reference ;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::reverse_iterator reverse_iterator;
  typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

  
 public:
  arena<N, MAX_SIZE, ID, MemPolicy> &a;
  std::function<void(std::string)> print;   
  std::vector<T, short_alloc<T, N, MAX_SIZE, ID, MemPolicy>> vec;


 SmallVector()
   : a(ArenaSingleton<arena<N, MAX_SIZE, ID, MemPolicy>>::GetInstance()), vec(a)
   {}
    
 SmallVector(size_t n)
   : a(ArenaSingleton<arena<N, MAX_SIZE, ID, MemPolicy>>::GetInstance()), vec(n, a)
    {}

  
 SmallVector(size_t n, std::function<void(std::string)> f) : a(ArenaSingleton<arena<N, MAX_SIZE, ID, MemPolicy>>::GetInstance()), vec(n, a), print(f) {};


    reference operator[](size_type pos){
    return vec[pos];
  }

    ~SmallVector(){
      std::cout << "destroying small vector" << std::endl;
    }
  
};
    //this is probably not working  
    // 

    /*  
    //ELEMENT ACCESS
  reference at(size_type pos){
    return vec.at(pos);
  }
  const_reference at(size_type pos) const{
    return vec.at(pos);
  }

  reference operator[](size_type pos){
    return vec[pos];
  }

  const_reference operator[](size_type pos) const{
    return vec[pos];
  }

  reference front(){
    return vec.front();
  }

  const_reference front() const{
    return vec.front();
  }

  reference back(){
    return vec.back();
  }

  const_reference back() const{
    return vec.back();
  }

  value_type* data(){
    return vec.data();
  }
  
  const value_type* data() const{
    return vec.data();
  }  


  //ITERATORS
  //SKIPPED

  iterator begin() noexcept{
    return vec.begin();
  }

  const_iterator begin() const noexcept{
    return vec.begin();
  }

  const_iterator cbegin() const noexcept{
    return vec.cbegin();
  }

  iterator end() noexcept{
    return vec.end();
  }

  const iterator end() const noexcept{
    return vec.end();
  }

  const_iterator cend() const noexcept {
    return vec.cend();
  }

  
  iterator rbegin() noexcept{
    return vec.rbegin();
  }

  const_iterator rbegin() const noexcept{
    return vec.rbegin();
  }

  const_iterator crbegin() const noexcept{
    return vec.crbegin();
  }


  iterator rend() noexcept{
    return vec.rend();
  }

  const iterator rend() const noexcept{
    return vec.rend();
  }

  const_iterator crend() const noexcept {
    return vec.crend();
  }   
  
  //CAPACITY
  bool empty() const noexcept
  {
    return vec.empty();
  }

  size_type size() const noexcept
  {
    return vec.size();
  }

  //TODO
  size_type max_size() const noexcept
  {
    return vec.max_size();
  }

  //TODO
  void reserve(size_type new_cap)
  {
    vec.reserve(new_cap);
  }

  //TODO
  size_type capacity() const noexcept
  {
    return vec.capacity();
  }

  //TODO
  void shrink_to_fit()
  {
    vec.shrink_to_fit();
  }

  //MODIFIERS
  void clear() noexcept
  {
    vec.clear();
  }

  iterator insert( const_iterator pos, const T& value ){
    return vec.insert(pos, value);
  }
  iterator insert( const_iterator pos, T&& value ){
    return vec.insert(pos, value);
  }
 
  iterator insert( const_iterator pos, size_type count, const T& value ){
    return vec.insert(pos, count, value);
  }

  template< class InputIt >
    iterator insert( const_iterator pos, InputIt first, InputIt last ){
    return vec.insert(pos, first, last);
  }

  iterator insert( const_iterator pos, std::initializer_list<T> ilist ){
    return vec.insert(pos, ilist);
  }

  template<class... Args>
    iterator emplace( const_iterator pos, Args&&...args){
    return vec.emplace(pos, args...);
  }

  iterator erase(iterator pos){
    return vec.erase(pos);
  }

  iterator erase(const_iterator pos){
    return vec.erase(pos);
  }

  iterator erase(iterator first, iterator last){
    return vec.erase(first, last);
  }

  iterator erase(const_iterator first, const_iterator last){
    return vec.erase(first, last);
  }

  void push_back(const T& value){
    vec.push_back(value);
  }

  void push_back( T&& value){
    vec.push_back(value);
  }

  template<class... Args>
    void emplace_back(Args&&... args){
    vec.emplace_back(args...);
  }

  void pop_back(){
    vec.pop_back();
  }

  void resize(size_type count){
    vec.resize(count);
  }

  void resize(size_type count, const value_type& value){
    vec.resize(count, value);
  }

  void swap(std::vector<T>& other){
    vec.swap(other);
  }
  
};
*/

  #endif  //   SMALL_VECTOR_H

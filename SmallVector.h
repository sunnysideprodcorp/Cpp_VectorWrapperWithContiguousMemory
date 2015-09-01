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
 public:
  std::function<void(std::string)> print;   

  // SmallVector always draws from a singleton arena to ensure memory is contiguous
  // for all SmallVectors with identical template parameters. int ID can be adjusted
  // to create nearly-identical SmallVector types with separate memory areas
 SmallVector ()
   : a(ArenaSingleton<Arena<N, MAX_SIZE, ID, MemPolicy>>::GetInstance()), m_vec(a)
   {}
 SmallVector ( size_t n )
   : a(ArenaSingleton<Arena<N, MAX_SIZE, ID, MemPolicy>>::GetInstance()), m_vec(n, a)
    {}
 SmallVector (size_t n, std::function<void(std::string)> f)
   : a(ArenaSingleton<Arena<N, MAX_SIZE, ID, MemPolicy>>::GetInstance()), m_vec(n, a), print(f) {};

  // Standard typedefs required to mimic std::vector
  typedef  T value_type;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type& reference ;
  typedef const  value_type& const_reference ;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::reverse_iterator reverse_iterator;
  typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;
 
  // Most vector functionality is made availability as a SmallVector member function 
  // to minimize friction when substituing SmallVector for std::vector
  reference at(size_type pos){
    return m_vec.at(pos);
  }
  const_reference at(size_type pos) const{
    return m_vec.at(pos);
  }

  reference operator[](size_type pos){
    return m_vec[pos];
  }

  const_reference operator[](size_type pos) const{
    return m_vec[pos];
  }

  reference front(){
    return m_vec.front();
  }

  const_reference front() const{
    return m_vec.front();
  }

  reference back(){
    return m_vec.back();
  }

  const_reference back() const{
    return m_vec.back();
  }

  value_type* data(){
    return m_vec.data();
  }
  
  const value_type* data() const{
    return m_vec.data();
  }  

  iterator begin() noexcept{
    return m_vec.begin();
  }

  const_iterator begin() const noexcept{
    return m_vec.begin();
  }

  const_iterator cbegin() const noexcept{
    return m_vec.cbegin();
  }

  iterator end() noexcept{
    return m_vec.end();
  }

  const iterator end() const noexcept{
    return m_vec.end();
  }

  const_iterator cend() const noexcept {
    return m_vec.cend();
  }

  
  iterator rbegin() noexcept{
    return m_vec.rbegin();
  }

  const_iterator rbegin() const noexcept{
    return m_vec.rbegin();
  }

  const_iterator crbegin() const noexcept{
    return m_vec.crbegin();
  }


  iterator rend() noexcept{
    return m_vec.rend();
  }

  const iterator rend() const noexcept{
    return m_vec.rend();
  }

  const_iterator crend() const noexcept {
    return m_vec.crend();
  }   
  
  bool empty() const noexcept
  {
    return m_vec.empty();
  }

  size_type size() const noexcept
  {
    return m_vec.size();
  }

  size_type max_size() const noexcept
  {
    return m_vec.max_size();
  }

  void reserve(size_type new_cap)
  {
    m_vec.reserve(new_cap);
  }

  size_type capacity() const noexcept
  {
    return m_vec.capacity();
  }

  void shrink_to_fit()
  {
    m_vec.shrink_to_fit();
  }

  void clear() noexcept
  {
    m_vec.clear();
  }

  iterator insert( const_iterator pos, const T& value ){
    return m_vec.insert(pos, value);
  }
  iterator insert( const_iterator pos, T&& value ){
    return m_vec.insert(pos, value);
  }
 
  iterator insert( const_iterator pos, size_type count, const T& value ){
    return m_vec.insert(pos, count, value);
  }

  template< class InputIt >
    iterator insert( const_iterator pos, InputIt first, InputIt last ){
    return m_vec.insert(pos, first, last);
  }

  iterator insert( const_iterator pos, std::initializer_list<T> ilist ){
    return m_vec.insert(pos, ilist);
  }

  template<class... Args>
    iterator emplace( const_iterator pos, Args&&...args){
    return m_vec.emplace(pos, args...);
  }

  iterator erase(iterator pos){
    return m_vec.erase(pos);
  }

  iterator erase(const_iterator pos){
    return m_vec.erase(pos);
  }

  iterator erase(iterator first, iterator last){
    return m_vec.erase(first, last);
  }

  iterator erase(const_iterator first, const_iterator last){
    return m_vec.erase(first, last);
  }

  void push_back(const T& value){
    m_vec.push_back(value);
  }

  void push_back( T&& value){
    m_vec.push_back(value);
  }

  template<class... Args>
    void emplace_back(Args&&... args){
    m_vec.emplace_back(args...);
  }

  void pop_back(){
    m_vec.pop_back();
  }

  void resize(size_type count){
    m_vec.resize(count);
  }

  void resize(size_type count, const value_type& value){
    m_vec.resize(count, value);
  }

  void swap(std::vector<T>& other){
    m_vec.swap(other);
  }
  
  
 private:
  Arena<N, MAX_SIZE, ID, MemPolicy> &a;  // always initialized with ArenaSingleton factory method
  std::vector<T, ShortAlloc<T, N, MAX_SIZE, ID, MemPolicy>> m_vec; // always initialized by referencing result of Arena a, constructed with ArenaSingleton factory method

};


#endif  //   SMALL_VECTOR_H

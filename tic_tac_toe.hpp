#pragma once
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <algorithm>
namespace tic_tac_toe
{
  using std::none_of;
  using std::endl;
  using std::vector;
  using std::cin;
  using std::cout;
  using std::ostream;
  struct TTT
  {
    enum { O, X, empty }square[ 9 ], cur_piece;
    enum { O_turn, X_turn, draw, O_win, X_win } game_stat;
    TTT( )
    {
      for ( size_t i = 0; i < 9; ++i ) { square[ i ] = empty; }
      game_stat = O_turn;
      cur_piece = O;
    }
    bool is_end(  ) const { return game_stat != O_turn && game_stat != X_turn; }
    bool can_place( size_t i ) const { return ( i < 9 ) && ( square[ i ] == empty ) && ! is_end( ); }
    void place( size_t i )
    {
      assert( can_place( i ) );
      square[ i ] = cur_piece;
      check_game( i );
      if ( ! is_end( ) ) { update( ); }
    }
    void update( )
    {
      assert( ! is_end( ) );
      if ( game_stat == O_turn ) { game_stat = X_turn; cur_piece = X; }
      else { game_stat = O_turn; cur_piece = O; }
    }
    void check_game( size_t last_place )
    {
      if ( ( square[ last_place % 3 ] == cur_piece &&
             square[ last_place % 3 + 3 ] == cur_piece &&
             square[ last_place % 3 + 6 ] == cur_piece ) ||
           ( square[ last_place / 3 * 3 ] == cur_piece &&
             square[ last_place / 3 * 3 + 1 ] == cur_piece &&
             square[ last_place / 3 * 3 + 2 ] == cur_piece ) ||
           ( square[ 0 ] == cur_piece &&
             square[ 4 ] == cur_piece &&
             square[ 8 ] == cur_piece ) ||
           ( square[ 2 ] == cur_piece &&
             square[ 4 ] == cur_piece &&
             square[ 6 ] == cur_piece )
           ) { game_stat = win( game_stat ); }
      else if ( none_of( & square[ 0 ], & square[ 9 ], [ ]( decltype( cur_piece ) & cp )->bool{ return cp == empty; }) ) { game_stat = draw; }
    }
    decltype( game_stat ) win( const decltype( game_stat ) & gs ) const
    {
      if ( gs == O_turn ) { return O_win; }
      else { assert( gs == X_turn ); return X_win; }
    }
    friend ostream & operator << ( ostream & os, const TTT & t )
    {
      for ( size_t i = 0; i < 9; i+=3 ) { os << to_char( t.square[ i ] ) << '|' << to_char( t.square[ i + 1 ] ) << '|' << to_char( t.square[ i +2 ] ) << endl; }
      return os;
    }
    static char to_char( const decltype( cur_piece ) & p )
    {
      if ( p == O ) { return 'O'; }
      else if ( p == X ) { return 'X'; }
      else { assert( p == empty ); return ' '; }
    }
    decltype( game_stat ) final_res( ) const
    {
      if ( is_end( ) ) { return game_stat; }
      else
      {
        auto bm( get_best_move( ) );
        TTT next( * this );
        next.place( bm );
        return next.final_res( );
      }
    }
    size_t get_best_move( ) const
    {
      vector< TTT > vt;
      vector< size_t > vm;
      for ( size_t i = 0; i < 9; ++i )
      {
        if ( can_place( i ) )
        {
          TTT next( * this );
          next.place( i );
          vt.push_back( next );
          vm.push_back( i );
        }
      }
      vector< decltype( game_stat ) > gs( vt.size( ) );
      transform( vt.begin( ), vt.end( ), gs.begin( ), [ ]( const TTT & t )->decltype( game_stat ){ return t.final_res( ); } );
      if ( find( gs.begin( ), gs.end( ), win( game_stat ) ) != gs.end( ) ) { return vm[ distance( gs.begin( ), find( gs.begin( ), gs.end( ), win( game_stat ) ) ) ]; }
      else if ( find( gs.begin( ), gs.end( ), draw ) != gs.end( ) ) { return vm[ distance( gs.begin( ), find( gs.begin( ), gs.end( ), draw ) ) ]; }
      else { return vm.front( ); }
    }
  };

  int example( bool player_turn )
  {
    TTT g;
    while ( ! g.is_end( ) )
    {
      if ( player_turn )
      {
        size_t i;
        cin >> i;
        if ( g.can_place( i ) )
        {
          g.place( i );
          player_turn = false;
        }
        else { cout << "invalid pos\n"; }
      }
      else
      {
        g.place( g.get_best_move( ) );
        player_turn = true;
      }
      cout << g << endl;
    }
    return 0;
  }

}

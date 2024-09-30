// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/
//

#include <daw/daw_assume.h>
#include <daw/deprecated/daw_vector.h>
#include <daw/vector.h>

#include <benchmark/benchmark.h>
#include <cstddef>
#include <vector>

template<typename T>
static void bench_vec_pb( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_resize_data_ptr( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.resize( n );
			T *ptr = v.data( );
			for( std::size_t i = 0; i < n; ++i ) {
				ptr[i] = T( i );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_reserve_pb( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_reserve_pb_assume( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				DAW_ASSUME( v.size( ) < v.capacity( ) );
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_mmap( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_mmap_reserve_pb( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( &v );
			v.reserve( n );
			for( std::size_t i = 0; i < n; ++i ) {
				v.push_back( T( i ) );
			}
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_resize_and_overwrite_assign( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_and_overwrite( n, []( T *ptr, std::size_t const N ) {
				for( std::size_t m = 0; m < N; ++m ) {
					ptr[m] = m;
				}
				return N;
			} );
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_resize_and_overwrite( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_and_overwrite( n, []( T *ptr, std::size_t const N ) {
				for( std::size_t m = 0; m < N; ++m ) {
					std::construct_at<T>( ptr + m, m );
				}
				return N;
			} );
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_resize_and_overwrite_alloc( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, std::allocator<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_and_overwrite(
			  n, []<typename Alloc>( T *ptr, std::size_t const N, Alloc alloc ) {
				  for( std::size_t m = 0; m < N; ++m ) {
					  std::allocator_traits<Alloc>::construct( alloc, ptr + m, m );
				  }
				  return N;
			  } );
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_vec_mmap_resize_and_overwrite( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			daw::vector<T, daw::MMapAlloc<T>> v;
			benchmark::DoNotOptimize( v.data( ) );
			v.resize_and_overwrite( n, []( T *ptr, std::size_t const N ) {
				for( std::size_t m = 0; m < N; ++m ) {
					ptr[m] = T( m );
				}
				return N;
			} );
			benchmark::DoNotOptimize( v.data( ) );
			benchmark::ClobberMemory( );
		}
		( );
	}
}

template<typename T>
static void bench_realloc_from_1( benchmark::State &s ) {
	auto n = std::size_t( s.range( 0 ) );
	for( auto _ : s ) {
		[=]( ) __attribute__( ( noinline ) ) {
			T *v = static_cast<T *>( malloc( sizeof( T ) * 1 ) );
			if( not v ) {
				std::abort( );
			}
			v[0] = 0;
			std::size_t cur_size = 2;
			auto last_size = 1;
			while( cur_size < n ) {
				auto alloc_size = cur_size * sizeof( T );
				v = static_cast<T *>( realloc( v, alloc_size ) );
				if( not v ) {
					std::abort( );
				}
				for( size_t m = last_size; m < cur_size; ++m ) {
					v[m] = m;
				}
				last_size = cur_size;
				cur_size = ( cur_size * 3 ) / 2;
			}
			auto alloc_size = n * sizeof( T );
			v = static_cast<T *>( realloc( v, alloc_size ) );
			for( size_t m = last_size; m < n; ++m ) {
				v[m] = m;
			}
			benchmark::ClobberMemory( );
			free( v );
		}
		( );
	}
}

template<std::size_t N>
static void make_args( benchmark::internal::Benchmark *b ) {
	b->Arg( N );
}
/*
// unsigned char
BENCHMARK_TEMPLATE( bench_vec_pb, unsigned char )->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, unsigned char )->Apply(
make_args<4> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb, unsigned char
)->Apply( make_args<4> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume,
unsigned char )->Apply( make_args<4> ); BENCHMARK_TEMPLATE(
bench_vec_resize_and_overwrite, unsigned char )
  ->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, unsigned char )
  ->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, unsigned char )
  ->Apply( make_args<4> );


BENCHMARK_TEMPLATE( bench_vec_pb, unsigned char )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, unsigned char )->Apply(
make_args<16> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb, unsigned char
)->Apply( make_args<16> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume,
unsigned char )->Apply( make_args<16> ); BENCHMARK_TEMPLATE(
bench_vec_resize_and_overwrite, unsigned char )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, unsigned char )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, unsigned char )
  ->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, unsigned char )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, unsigned char )->Apply(
make_args<1024> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb, unsigned char
)->Apply( make_args<1024> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume,
unsigned char )->Apply( make_args<1024> ); BENCHMARK_TEMPLATE(
bench_vec_resize_and_overwrite, unsigned char )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, unsigned char )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, unsigned char )
  ->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, unsigned char )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, unsigned char )->Apply(
make_args<4096> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb, unsigned char
)->Apply( make_args<4096> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume,
unsigned char )->Apply( make_args<4096> ); BENCHMARK_TEMPLATE(
bench_vec_resize_and_overwrite, unsigned char )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, unsigned char )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, unsigned char )
  ->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, unsigned char )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, unsigned char )->Apply(
make_args<16384> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb, unsigned char
)->Apply( make_args<16384> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume,
unsigned char )->Apply( make_args<16384> ); BENCHMARK_TEMPLATE(
bench_vec_resize_and_overwrite, unsigned char )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, unsigned char )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, unsigned char )
  ->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, unsigned char )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, unsigned char )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, unsigned char )->Apply(
make_args<5242880> ); BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume, unsigned
char )->Apply( make_args<5242880> ); BENCHMARK_TEMPLATE(
bench_vec_resize_and_overwrite, unsigned char )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, unsigned char )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, unsigned char )
  ->Apply( make_args<5242880> );
*/
// int
BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume, int )->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )
  ->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, int )
  ->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, int )
  ->Apply( make_args<4> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, int )->Apply( make_args<4> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume, int )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, int )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, int )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, int )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume, int )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, int )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, int )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, int )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume, int )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, int )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, int )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, int )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume, int )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, int )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, int )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, int )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, int )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, int )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, int )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb_assume, int )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, int )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, int )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, int )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, int )->Apply( make_args<5242880> );

// long long
BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, long long )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, long long )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, long )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, long long )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, long long )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, long )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, long long )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, long long )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, long )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, long long )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, long long )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, long )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, long long )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, long long )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, long long )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, long long )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, long long )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, long long )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, long )->Apply( make_args<5242880> );

// float
BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, float )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, float )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, float )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, float )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, float )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, float )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, float )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, float )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, float )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, float )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, float )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, float )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, float )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, float )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, float )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, float )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, float )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, float )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, float )->Apply( make_args<5242880> );

// double
BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, double )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, double )
  ->Apply( make_args<16> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, double )->Apply( make_args<16> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, double )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, double )
  ->Apply( make_args<1024> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, double )->Apply( make_args<1024> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, double )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, double )
  ->Apply( make_args<4096> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, double )->Apply( make_args<4096> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, double )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, double )
  ->Apply( make_args<16384> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, double )->Apply( make_args<16384> );

BENCHMARK_TEMPLATE( bench_vec_pb, double )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_data_ptr, double )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_reserve_pb, double )->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite, double )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_assign, double )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_vec_resize_and_overwrite_alloc, double )
  ->Apply( make_args<5242880> );
BENCHMARK_TEMPLATE( bench_realloc_from_1, double )->Apply( make_args<5242880> );

 
/* ========================================

   * File Name : commons.h

   * Creation Date : 13-08-2015

   * Last Modified : Fri 11 Dec 2015 07:15:55 PM CET

   * Created By : Karel Ha <mathemage@gmail.com>

   * Description: common includes, typedefs, defines and constants
   ==========================================*/

#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <cstdio>
#include <stdint.h>
#include <cstdlib>
#include "tbb/tick_count.h"
#include <vector>

typedef uint16_t length_t;
typedef uint32_t offset_t;
typedef char byte_t;

using namespace std;

extern FILE *outstream;

#endif

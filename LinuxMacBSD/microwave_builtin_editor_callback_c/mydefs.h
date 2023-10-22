// Project/System dependant defines and typedefs

#ifndef __MYDEFS_H__
#define __MYDEFS_H__

/* This macro is used to retrieve an enclosing structure from a pointer to a nested element
 * source: https://en.wikipedia.org/wiki/Offsetof
 *         https://stackoverflow.com/questions/15832301/understanding-container-of-macro-in-the-linux-kernel
 * When you use the cointainer_of macro, you want to retrieve the structure that contains the pointer of a given field. For example:
 * @code
 * struct numbers {
 *     int one;
 *     int two;
 *     int three;
 * } n;
 * @endcode
 * int *ptr = &n.two;
 * struct numbers *n_ptr;
 * n_ptr = container_of(ptr, struct numbers, two);
 * You have a pointer that points in the middle of a structure (and you know that is a 
 * pointer to the filed two [the field name in the structure]), but you want to retrieve 
 * the entire structure (numbers).
 */

#define container_of(ptr, type, member) ((type *)((char *)(1 ? (ptr) : &((type *)0)->member) - offsetof(type, member)))

#define OVEN_MAX_CB (10U)

#endif

/** @file
 * @brief リソースマクロ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_RESOURCE_H
#define GRAPHENE_RESOURCE_H

#ifdef __cplusplus
extern "C" {
#endif

#define RESOURCE_STRING(x) #x
#define RESOURCE_SYMBOL(x) extern const char x[] asm(RESOURCE_STRING(_##x))
#define RESOURCE_EXTERN(x) RESOURCE_SYMBOL(binary_##x##_start); RESOURCE_SYMBOL(binary_##x##_end)

#define RESOURCE_DATA(x) (binary_##x##_start)
#define RESOURCE_SIZE(x) (binary_##x##_end - binary_##x##_start)

#ifdef __cplusplus
}
#endif

#endif // GRAPHENE_RESOURCE_H

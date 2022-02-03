/** @file
 * @brief セットアップ(GLFW)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_SETUP_GLFW_HPP
#define GRAPHENE_SETUP_GLFW_HPP

namespace Graphene::Setup {

/**
 * @brief GLFWの初期化
 *
 * GLFWを初期化します。 @n
 * 初期化に失敗した場合は例外を送出します。
 *
 * @throw std::exception 初期化失敗
 * @return なし
 */
void InitializeGLFW(void);

} // namespace Graphene::Setup

#endif // GRAPHENE_SETUP_GLFW_HPP

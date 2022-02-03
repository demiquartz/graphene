/** @file
 * @brief グラフェンライブラリ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_HPP
#define GRAPHENE_HPP

#include <cstddef>

namespace Graphene {

/**
 * @brief ライブラリの初期化
 *
 * ライブラリを初期化します。 @n
 * 初期化に失敗した場合は例外を送出します。
 *
 * @return なし
 * @throw std::exception 初期化失敗
 */
void Initialize(void);

/**
 * @brief バージョン情報の取得
 *
 * バージョン情報を取得します。
 *
 * @return バージョン情報
 */
const char* GetVersion(void) noexcept;

/**
 * @brief メジャーバージョンの取得
 *
 * メジャーバージョンを取得します。
 *
 * @return メジャーバージョン
 */
std::size_t GetMajorVersion(void) noexcept;

/**
 * @brief マイナーバージョンの取得
 *
 * マイナーバージョンを取得します。
 *
 * @return マイナーバージョン
 */
std::size_t GetMinorVersion(void) noexcept;

/**
 * @brief パッチバージョンの取得
 *
 * パッチバージョンを取得します。
 *
 * @return パッチバージョン
 */
std::size_t GetPatchVersion(void) noexcept;

} // Graphene

#endif // GRAPHENE_HPP

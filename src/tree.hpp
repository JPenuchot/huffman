#pragma once

#include <functional>
#include <variant>
#include <memory>

using namespace std;

/**
 * @brief      Représente un arbre (avec pas beaucoup de contrôles parce-que
 * franchement osef pour un TP)
 *
 * @tparam     T     Type contenu dans l'arbre
 */
template<typename T>
struct tree
{
  // l et r contiennent chacun soit un pointeur vers un sous-arbre,
  // soit un élément de type T.

  //  std::variant est un équivalent de union mais syntaxiquement plus léger,
  //  il permet aussi d'accéder à l'index du type contenu lors du runtime
  //  (0 si T, 1 si shared_ptr<tree<T>>).
  //  
  //  Voir : http://en.cppreference.com/w/cpp/utility/variant
  variant<T, shared_ptr<tree<T>>> l;
  variant<T, shared_ptr<tree<T>>> r;
};

/**
 * @brief      Désigne la gauche ou la droite dans un arbre.
 */
enum side { left, right };

template<typename T>
inline void iter( shared_ptr<tree<T>>     tr
                , function<void(T, side)> leaf
                , function<void(side)>    node
                , function<void()>        unstack
                )
{
  //  On vérifie le type contenu dans le std::variant
  if(tr->l.index() == 0)
    leaf(get<T>(tr->l), side::left);
  else
  { node(side::left);
    iter( get<shared_ptr<tree<T>>>(tr->l) , leaf, node , unstack ); }

  unstack();

  //  Idem, on vérifie le type contenu...
  if (tr->r.index() == 0)
    leaf(get<T>(tr->r), side::right);
  else
  { node(side::right);
    iter( get<shared_ptr<tree<T>>>(tr->r) , leaf , node , unstack ); }

  unstack();
}

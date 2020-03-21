//
// Created by tudom on 2020-03-21.
//

#pragma once

#include <type_traits>
#include <utility>

/**
 * Main namespace for universal
 * utilities in the Info* libraries
 *
 * InfoUtils works with this namespace
 */
namespace info {
  /**
   * \brief Generalizes lambdas by allowing recursion
   *
   * A lambda construct wrapper allowing the creation of recursive lambdas.
   *
   * \note The first parameter must be an `auto` parameter representing the
   *        lambda itself. In Info project it is always called `self`.
   *
   * \warning Despite the lambda declaration having to contain a bonus parameter,
   *           it does not change the output lambda's interface. Therefore, it
   *           must not be passed when invoking the lambda.
   *
   * \note While lambda is used throughout this documentation, any functor suffices.
   *        Of course, other non-lambda objects don't really require this helper
   *        to allow for recursion.
   *
   * \tparam F The unspecified lambda type to allow recursion for
   *
   * \since 1.0
   * \author bodand
   */
  template<class F>
  struct lambda {
      /**
       * \brief Calls the lambda with the passed arguments, plus itself for recursion
       *
       * Calls the lambda and passes this object with the other arguments
       * passed, therefore, allowing recursion by calling the first parameter.
       *
       * The passed parameters are forwarded.
       *
       * \note If the invocation of the lambda is nothrow then this function
       *        is nothrow.
       *
       * \tparam ArgsT The types of the arguments
       *
       * \param args The arguments to pass to the lambda
       *
       * \return The return value of the lambda. May be void.
       */
      template<class... ArgsT>
      auto operator()(ArgsT&& ... args) noexcept(std::is_nothrow_invocable_v<F, ArgsT...>) {
          return _fun(*this, std::forward<ArgsT>(args)...);
      }

      /**
       * \brief Constructs a info::lambda object from any lambda type
       *
       * \param fun The lambda of unspecified type. This is the lambda upon
       *             which the power of recursion is to be bestowed
       */
      explicit lambda(F fun) noexcept
             : _fun{fun} {};

  private:
      F _fun; ///< The real function object
  };
}

/**
 * @file constraints.hpp
 * @author Manish Kumar
 *
 * Declaration of the Constraints class.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_LMNN_CONSTRAINTS_HPP
#define MLPACK_METHODS_LMNN_CONSTRAINTS_HPP

#include <mlpack/prereqs.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>

namespace mlpack {
namespace lmnn {
/**
 * Interface for generating distance based constraints on a given
 * dataset, provided corresponding true labels and a quantity parameter (k)
 * are specified.
 *
 * Class provides TargetNeighbors() (Used for calculating target neighbors
 * of each data point), Impostors() (used for calculating impostors of each
 * data point) and Triplets() (Generates sets of {dataset, target neighbors,
 * impostors} tripltets.)
 */
template<typename MetricType = metric::SquaredEuclideanDistance>
class Constraints
{
 public:
  //! Convenience typedef.
  typedef neighbor::NeighborSearch<neighbor::NearestNeighborSort, MetricType>
      KNN;

  /**
   * Constructor for creating a Constraints instance.
   *
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   * @param k Number of target neighbors, impostors & triplets.
   */
  Constraints(const arma::mat& dataset,
              const arma::Row<size_t>& labels,
              const size_t k);

  /**
   * Calculates k similar labeled nearest neighbors and stores them into the
   * passed matrix.
   *
   * @param outputMatrix Coordinates matrix to store target neighbors.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   */
  void TargetNeighbors(arma::Mat<size_t>& outputMatrix,
                       const arma::mat& dataset,
                       const arma::Row<size_t>& labels);

  /**
   * Calculates k similar labeled nearest neighbors for a batch of dataset and
   * stores them into the passed matrix.
   *
   * @param outputMatrix Coordinates matrix to store target neighbors.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   * @param begin Index of the initial point of dataset.
   * @param batchSize Number of data points to use.
   */
  void TargetNeighbors(arma::Mat<size_t>& outputMatrix,
                       const arma::mat& dataset,
                       const arma::Row<size_t>& labels,
                       const size_t begin,
                       const size_t batchSize);

  /**
   * Calculates k differently labeled nearest neighbors for each datapoint and
   * writes them back to passed matrix.
   *
   * @param outputMatrix Coordinates matrix to store impostors.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   */
  void Impostors(arma::Mat<size_t>& outputMatrix,
                 const arma::mat& dataset,
                 const arma::Row<size_t>& labels);

  /**
   * Calculates k differently labeled nearest neighbors & distances to
   * impostors for each datapoint and writes them back to passed matrices.
   *
   * @param outputNeighbors Coordinates matrix to store impostors.
   * @param outputDistance matrix to store distance.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   */
  void Impostors(arma::Mat<size_t>& outputNeighbors,
                 arma::mat& outputDistance,
                 const arma::mat& dataset,
                 const arma::Row<size_t>& labels);

  /**
   * Calculates k differently labeled nearest neighbors for a batch of dataset
   * and writes them back to passed matrix.
   *
   * @param outputMatrix Coordinates matrix to store impostors.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   * @param begin Index of the initial point of dataset.
   * @param batchSize Number of data points to use.
   */
  void Impostors(arma::Mat<size_t>& outputMatrix,
                 const arma::mat& dataset,
                 const arma::Row<size_t>& labels,
                 const size_t begin,
                 const size_t batchSize);

  /**
   * Calculates k differently labeled nearest neighbors & distances to
   * impostors for a batch of dataset and writes them back to passed matrices.
   *
   * @param outputNeighbors Coordinates matrix to store impostors.
   * @param outputDistance matrix to store distance.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   * @param begin Index of the initial point of dataset.
   * @param batchSize Number of data points to use.
   */
  void Impostors(arma::Mat<size_t>& outputNeighbors,
                 arma::mat& outputDistance,
                 const arma::mat& dataset,
                 const arma::Row<size_t>& labels,
                 const size_t begin,
                 const size_t batchSize);

  /**
   * Calculates k differently labeled nearest neighbors & distances to
   * impostors for some points of dataset and writes them back to passed
   * matrices.
   *
   * @param outputNeighbors Coordinates matrix to store impostors.
   * @param outputDistance matrix to store distance.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   * @param points Indices of data points to calculate impostors on.
   */
  void Impostors(arma::Mat<size_t>& outputNeighbors,
                 arma::mat& outputDistance,
                 const arma::mat& dataset,
                 const arma::Row<size_t>& labels,
                 const arma::uvec& points);

  /**
   * Generate triplets {i, j, l} for each datapoint i and writes back generated
   * triplets to matrix passed.
   *
   * @param outputMatrix Coordinates matrix to store triplets.
   * @param dataset Input dataset.
   * @param labels Input dataset labels.
   */
  void Triplets(arma::Mat<size_t>& outputMatrix,
                const arma::mat& dataset,
                const arma::Row<size_t>& labels);

  //! Get the number of target neighbors (k).
  const size_t& K() const { return k; }
  //! Modify the number of target neighbors (k).
  size_t& K() { return k; }

  //! Access the boolean value of precalculated.
  const bool& PreCalulated() const { return precalculated; }
  //! Modify the value of precalculated.
  bool& PreCalulated() { return precalculated; }

 private:
  //! Number of target neighbors & impostors to calulate.
  size_t k;

  //! Store unique labels.
  arma::Row<size_t> uniqueLabels;

  //! Store indices of data points having similar label.
  std::vector<arma::uvec> indexSame;

  //! Store indices of data points having different label.
  std::vector<arma::uvec> indexDiff;

  //! False if nothing has ever been precalculated.
  bool precalculated;

  /**
  * Precalculate the unique labels, and indices of similar
  * and different datapoints on the basis of labels.
  */
  inline void Precalculate(const arma::Row<size_t>& labels);
};

} // namespace lmnn
} // namespace mlpack

// Include implementation.
#include "constraints_impl.hpp"

#endif

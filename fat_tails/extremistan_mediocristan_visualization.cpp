#include <iostream>
#include <fstream>
#include <cmath>   // for exp, sqrt, M_PI

/**
 * @file normal_distribution_generator.cpp
 * @brief Generates normal distribution data for Mediocristan vs Extremistan comparison
 * @date 2024-09-26
 * @author AI-generated
 *
 * This program demonstrates the difference between "Mediocristan" (single Gaussian)
 * and "Extremistan" (composite Gaussian) distributions. The implementation generates
 * data files suitable for visualization with xmgrace or qtgrace plotting tools.
 *
 * Based on discussions between Nassim Taleb and Stephen Wolfram on fat-tail
 * distributions and their practical implications in modeling real-world phenomena.
 */

 /**
  * @brief Calculates the probability density function of a normal distribution
  * @param x The value at which to evaluate the PDF
  * @param mean The mean (μ) of the distribution
  * @param stddev The standard deviation (σ) of the distribution
  * @return The PDF value at point x
  */
double normal_pdf(double x, double mean, double stddev) {
    const double pi = std::acos(-1);  // Calculate π (alternatively use M_PI if available)
    double exponent = -((x - mean) * (x - mean)) / (2 * stddev * stddev);
    double coefficient = 1.0 / (stddev * std::sqrt(2 * pi));
    return coefficient * std::exp(exponent);
}

int main() {
    double x = 0.0;       // Evaluation point for PDF calculation
    double mean = 0.0;    // Mean of both distributions (centered at origin)

    /**
     * MEDIOCRISTAN DISTRIBUTION
     *
     * In Mediocristan, extreme events result from the combination of multiple
     * independent, moderately-sized events rather than single large events.
     *
     * Example: Human height distribution
     * If two randomly selected humans have a combined height of 4.10m, the most
     * probable combination is approximately 2.05m + 2.05m rather than 4.0m + 0.1m.
     *
     * Characteristics:
     * - Single, constant standard deviation
     * - Smooth, predictable behavior
     * - Serves as baseline for comparison with fat-tail distributions
     */
    double mediocristan_stddev = 1.0;  // Standard deviation for baseline distribution

    /**
     * EXTREMISTAN DISTRIBUTION
     *
     * In Extremistan, extreme events are typically dominated by single large
     * tail events rather than combinations of moderate events. This creates
     * "fat tails" where extreme outcomes are more probable than in normal distributions.
     *
     * Example: Net worth distribution
     * If two randomly selected individuals have a combined net worth of $4.10 billion,
     * it's more likely this results from one billionaire plus an average person
     * rather than two individuals each worth ~$2 billion.
     *
     * Implementation: Composite of two Gaussians with different standard deviations
     * to model uncertainty in the volatility parameter itself (volatility of volatility).
     */
    double a = 0.6;                    // Tail thickness control parameter
    double stddev_1 = 1.0 - a;        // Lower standard deviation component (0.4)
    double stddev_2 = 1.0 + a;        // Higher standard deviation component (1.6)

    // Generate Mediocristan distribution data
    std::ofstream outfile1("normal_distribution.xvg");
    outfile1 << "# Mediocristan: Single Gaussian distribution (σ = " << mediocristan_stddev << ")\n";
    outfile1 << "# x-value\tPDF-value\n";
    for (double x = -5.0; x <= 5.0; x += 0.1) {
        outfile1 << x << " " << normal_pdf(x, mean, mediocristan_stddev) << std::endl;
    }

    // Generate Extremistan distribution data (composite Gaussian)
    std::ofstream outfile2("normal_distribution_comp.xvg");
    outfile2 << "# Extremistan: Composite Gaussian distribution\n";
    outfile2 << "# Equal mixture of N(0," << stddev_1 << "²) and N(0," << stddev_2 << "²)\n";
    outfile2 << "# x-value\tPDF-value\n";
    for (double x = -5.0; x <= 5.0; x += 0.1) {
        double composite_pdf = 0.5 * (normal_pdf(x, mean, stddev_2) + normal_pdf(x, mean, stddev_1));
        outfile2 << x << " " << composite_pdf << std::endl;
    }

    outfile1.close();
    outfile2.close();

    std::cout << "Distribution data files generated successfully:\n";
    std::cout << "- normal_distribution.xvg (Mediocristan)\n";
    std::cout << "- normal_distribution_comp.xvg (Extremistan)\n";
    std::cout << "Use xmgrace or qtgrace for visualization.\n";

    return 0;
}

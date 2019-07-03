

#ifndef __YOUBOT_GRASP_IK__
#define __YOUBOT_GRASP_IK__

#include <vector>
#include <algorithm>
#include <Eigen/Dense>

#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Vector3.h>

namespace ik_solver_service {

#define ALMOST_ONE 0.99999

struct joint_positions_solution_t
{
  double joints[5];
  bool feasible;
  bool arm_to_front;
  bool arm_bended_up;
  bool gripper_downwards;
};

class YoubotGraspIK
{
public:
  YoubotGraspIK()
  {

  }

  static joint_positions_solution_t solveClosestIK(joint_positions_solution_t current_joint_positions,
                                                   geometry_msgs::Point desired_position,
                                                   geometry_msgs::Vector3 desired_normal);

  static joint_positions_solution_t solvePreferredPitchIK(double preferred_pitch, geometry_msgs::Point desired_position,
                                                          geometry_msgs::Vector3 desired_normal);

  static joint_positions_solution_t solvePreferredTypeIK(bool arm_to_front, bool arm_bended_up, bool gripper_downwards,
                                                         geometry_msgs::Point desired_position,
                                                         geometry_msgs::Vector3 desired_normal);

  static joint_positions_solution_t solveFullyConstrainedIK(int id, double pitch, geometry_msgs::Point desired_position,
                                                            geometry_msgs::Vector3 desired_normal);
  static joint_positions_solution_t solvePitchRollIK(int id, double pitch, double roll, geometry_msgs::Point desired_position);

private:

  // find all solutions
  static std::vector<joint_positions_solution_t> getAllIKSolutions(geometry_msgs::Point desired_position,
                                                                   geometry_msgs::Vector3 desired_normal);

  static void calculateUniquePitchSolutions(std::vector<joint_positions_solution_t>& solutions,
                                            geometry_msgs::Point desired_position,
                                            geometry_msgs::Vector3 desired_normal);

  static void calculateDegenerativeSolutions(std::vector<joint_positions_solution_t>& solutions,
                                             geometry_msgs::Point desired_position);
  static bool existsUniquePitchSolution(geometry_msgs::Point desired_position, geometry_msgs::Vector3 desired_normal);

  // finding solution with minimum joint angle difference
  static joint_positions_solution_t takeClosestSolution(joint_positions_solution_t current_joint_positions,
                                                        std::vector<joint_positions_solution_t> solutions);
  static double getMaxJointDifference(joint_positions_solution_t solution_a, joint_positions_solution_t solution_b);

  // finding solution with closest gripper pitch
  static joint_positions_solution_t computeExactPitchSolution(double preferred_pitch,
                                                              geometry_msgs::Point desired_position,
                                                              geometry_msgs::Vector3 desired_normal);
  static joint_positions_solution_t takeClosestPitchSolution(double preferred_pitch,
                                                             std::vector<joint_positions_solution_t> solutions);
  static double getPitchDifference(double preferred_pitch, joint_positions_solution_t solution);

  // finding solution of desired type
  static joint_positions_solution_t takeClosestTypeSolution(bool arm_to_front, bool arm_bended_up,
                                                            bool gripper_downwards,
                                                            std::vector<joint_positions_solution_t> solutions);

  // finding unique pitch solution

  static bool checkSingleSolutionFeasability(joint_positions_solution_t single_solution);
  static joint_positions_solution_t computeSingleIKSolution(Eigen::Vector3d des_position, double des_roll,
                                                            double des_pitch, int id);
  // Geometric Constants
  static constexpr float lox_ = 0.033;
  static constexpr float loz_ = 0.1472;
  static constexpr float l_2_ = 0.155;
  static constexpr float l_3_ = 0.135;
  static constexpr float l_4_ = 0.218 + 0.13; // Including marker
};

double deg2Rad(double angle_in_deg);
double normalizeAngle(double angle);
static double sign(double value);

} // namespace ik_solver_service

#endif

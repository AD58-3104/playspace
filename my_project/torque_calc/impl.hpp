#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <fstream>
#include <optional>
#include <vector>
struct link{
    link& parent_;
    using child_link = link;
    std::optional<child_link&> roll_;
    std::optional<child_link&> pitch_;
    std::optional<child_link&> yaw_;
    double mass_;
    std::vector<double> inertia_;
    link(link& parent,const double& mass,const std::vector<double>& inertia):parent_(parent),roll_(),pitch_(),yaw_(),mass_(mass),inertia_(inertia) {
        static_assert("inertia must have three item",inertia.size() == 3);
    }
};


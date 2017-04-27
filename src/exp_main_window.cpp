#include "exp_main_window.h"
#include "ui_exp_main_window.h"

//===========================================================================
ExpMainWindow::ExpMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExpMainWindow)
{
    ui->setupUi(this);
}

//===========================================================================
ExpMainWindow::~ExpMainWindow()
{
    delete ui;
}

//===========================================================================
void ExpMainWindow::Init()
{
    // initialize subscribers
    human_pose2d_sub_ = nh_.subscribe<geometry_msgs::Pose2D>("/tracking/human_pose2d", 1,
                                                             &ExpMainWindow::human_pose_callback, this);

    // initialize publishers

    // get parameters

    // initialize variables
    human_pose_ = geometry_msgs::Pose2D();
    robot_pose_ = geometry_msgs::Pose2D();

    // connect signals and slots
    connect(this, SIGNAL(human_pose_received(double,double,double)),
            ui->gl_tracking_display, SLOT(set_human_pose(double,double,double)));
    connect(this, SIGNAL(robot_pose_received(double,double,double)),
            ui->gl_tracking_display, SLOT(set_robot_pose(double,double,double)));

    connect(&update_timer_, SIGNAL(timeout()), this, SLOT(update_gui_info()));
    connect(&update_timer_, SIGNAL(timeout()), ui->gl_tracking_display, SLOT(update()));
    update_timer_.start(30);
}

//===========================================================================
void ExpMainWindow::update_gui_info()
{
    // spin ros
    ros::spinOnce();

    // check for shutdown
    if (ros::isShuttingDown()) {
        this->close();
    }
}

//===========================================================================
void ExpMainWindow::human_pose_callback(const geometry_msgs::Pose2D_<std::allocator<void> >::ConstPtr &pose2d_msg)
{
    human_pose_.x = pose2d_msg->x;
    human_pose_.y = pose2d_msg->y;
    human_pose_.theta = pose2d_msg->theta;

    // send out the signal
    emit human_pose_received(human_pose_.x, human_pose_.y, human_pose_.theta);
}

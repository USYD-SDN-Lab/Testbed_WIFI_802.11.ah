#pragma once
#ifndef __SDN_LAB_SYSTEMMODEL_H
    #define __SDN_LAB_SYSTEMMODEL_H
    // headers
    #include <string>
    #include <fstream>
    // headers - ns3
    #include "ns3/node-container.h"
    #include "ns3/mobility-helper.h"
    // headers - 3rd party
    #include <cmath>                                                    // support math funtions: floor, ceil, sin, ... etc.
    #include "Modules/Toolbox/Error.h"                                  // Error
    // marcors
    // marcors - location distribution
    #define __SDN_LAB_SYSTEMMODEL_LOC_DIST_UNIFORM                  0   // all nodes are at the same point
    #define __SDN_LAB_SYSTEMMODEL_LOC_DIST_RANDOM                   1   // all nodes are randomly distributed
    // marcors - mobility
    #define __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_STATIC              0   // all nodes are fixed
    #define __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_UNIFORM_WALK        1   // all nodes walk at the same speed and at the same direction
    #define __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_WAYPOINT     2   // random waypoint model
    #define __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_WALK         3   // random walk model
    #define __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_DIRECTION    4   // random direction model
    namespace SdnLab{
        class SystemModel{
            protected:
            // debug
            std::string path = "";
            // ns3 parameter
            ns3::NodeContainer * stas = NULL;
            ns3::NodeContainer * ap = NULL;
            // types
            unsigned int typeLocDist = __SDN_LAB_SYSTEMMODEL_LOC_DIST_RANDOM;
            unsigned int typeMob = __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_STATIC;
            // AP coordinates
            double apX = 0;
            double apY = 0;
            // other settings
            double radius = 0;                                         // the radius of the circle
            double staPauseMin = 0;                                    // the minimal pause time, at seconds
            double staPauseMax = 0;                                    // the maximal pause time, at seconds
            double staSpeedMin = 0;                                    // the minimal of the node speed (not used when static), at m/s
            double staSpeedMax = 0;                                    // the maximal of the node speed (not used when static), at m/s
            double staFixedSpeedX = 0;                                 // the fixed speed on X (only used when uniform)
            double staFixedSpeedY = 0;                                 // the fixed speed on Y (only used when uniform)
            double staUniformLocX = 0;                                 // the uniform X of node coordinate (not used when nodes are separate)
            double staUniformLocY = 0;                                 // the uniform Y of node coordinate (not used when nodes are separate)

            public:
            /**
             * Constructor
             * @ap:         the container of AP
             * @stas:       the container of STAs
             * @radius:     the radius of the circle
             */
            SystemModel(ns3::NodeContainer * ap, ns3::NodeContainer * stas, double radius){
                if(!ap){
                    Toolbox::Error err("/Components", "SystemModel.h", "SystemModel", "SystemModel", "The pointer to the NodeContainer (AP) is empty.");
                    err.SetType2IllegalParameters();
                    throw err;
                }
                if(!stas){
                    Toolbox::Error err("/Components", "SystemModel.h", "SystemModel", "SystemModel", "The pointer to the NodeContainer (STA) is empty.");
                    err.SetType2IllegalParameters();
                    throw err;
                }
                if(radius <= 0 ){
                    Toolbox::Error err("/Components", "SystemModel.h", "SystemModel", "SystemModel", "The radius of SystemModel is not positive.");
                    err.SetType2IllegalParameters();
                    throw err;
                }
                // accept inputs
                this->stas = stas;
                this->radius = radius;
                // set ap coordinates
                this->apX = radius;
                this->apY = radius;
            };
            /**
             * Set the node location distribution type
             * @staUniformLocX: the uniform X of node coordinate (not used when nodes are separate)
             * @staUniformLocY: the uniform Y of node coordinate (not used when nodes are separate)
             */
            void LocDistType2Uniform(double staUniformLocX, double staUniformLocY){
                this->typeLocDist = __SDN_LAB_SYSTEMMODEL_LOC_DIST_UNIFORM;
                this->staUniformLocX = staUniformLocX;
                this->staUniformLocY = staUniformLocY;
            }
            void LocDistType2Random(){
                this->typeLocDist = __SDN_LAB_SYSTEMMODEL_LOC_DIST_RANDOM;
            }
            /**
             * Set the STA mobility type
             * @speed:          the uniform unchanged speed
             * @angle:          the uniform unchanged angle
             * @speedMin:       the minimal speed
             * @speedMax:       the maximal speed
             * @pause:          the pause time for all nodes
             */
            void MobilityType2Static(){
                this->typeMob = __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_STATIC;
            }
            void MobilityType2UniformWalk(double speed, double angle){
                this->typeMob = __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_UNIFORM_WALK;
                this->staFixedSpeedX = speed*cos(angle);
                this->staFixedSpeedY = speed*sin(angle);
            }
            void MobilityType2RandomWaypoint(double speedMin, double speedMax, double pauseMin, double pauseMax){
                this->typeMob = __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_WAYPOINT;
                this->staSpeedMin = speedMin;
                this->staSpeedMax = speedMax;
                this->staPauseMin = pauseMin;
                this->staPauseMax = pauseMax;
            }
            void MobilityType2RandomWalk(double speedMin, double speedMax, double pauseMin, double pauseMax){
                this->typeMob = __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_WALK;
                this->staSpeedMin = speedMin;
                this->staSpeedMax = speedMax;
                this->staPauseMin = pauseMin;
                this->staPauseMax = pauseMax;
            }
            void MobilityType2RandomDirection(double speedMin, double speedMax, double pauseMin, double pauseMax){
                this->typeMob = __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_DIRECTION;
                this->staSpeedMin = speedMin;
                this->staSpeedMax = speedMax;
                this->staPauseMin = pauseMin;
                this->staPauseMax = pauseMax;
            }

            /**
             * Init the model
             */
            void Init(){
                // set varables
                MobilityHelper mobAp;
                MobilityHelper mobSTAs;
                Ptr<ListPositionAllocator> posAP = NULL;
                Ptr<ListPositionAllocator> posSTAsUniform = NULL;
                // set varables - boundary setting (i.e., random postion allocator)
                Ptr<UniformDiscPositionAllocator> posSTAsRandom = CreateObject<UniformDiscPositionAllocator>();
                posSTAsRandom->SetRho(this->radius);
                posSTAsRandom->SetX(this->radius);
                posSTAsRandom->SetY(this->radius);
                // set variables - speed range
                Ptr<UniformRandomVariable> speedRange = CreateObject<UniformRandomVariable> ();
                speedRange->SetAttribute("Min", DoubleValue(this->staSpeedMin));
                speedRange->SetAttribute("Max", DoubleValue(this->staSpeedMax));
                // set variales - pause range
                Ptr<UniformRandomVariable> pauseRange = CreateObject<UniformRandomVariable> ();
                pauseRange->SetAttribute("Min", DoubleValue(this->staPauseMin));
                pauseRange->SetAttribute("Max", DoubleValue(this->staPauseMax));

                // set AP
                // set AP - location
                posAP = CreateObject<ListPositionAllocator> ();
                posAP->Add (Vector (this->radius, this->radius, 0));
                mobAp.SetPositionAllocator (posAP);
                // set AP - mobility
                mobAp.SetMobilityModel("ns3::ConstantPositionMobilityModel");
                mobAp.Install(*this->ap);
                // set STA
                // set STA - locations
                switch(this->typeLocDist){
                    case __SDN_LAB_SYSTEMMODEL_LOC_DIST_UNIFORM:
                        posSTAsUniform = CreateObject<ListPositionAllocator> ();
    	                posSTAsUniform->Add (Vector (this->staUniformLocX, this->staUniformLocY, 0));
    	                mobSTAs.SetPositionAllocator (posSTAsUniform);
                        break;
                    case __SDN_LAB_SYSTEMMODEL_LOC_DIST_RANDOM:                        
                        mobSTAs.SetPositionAllocator(posSTAsRandom);
                        break;
                }
                // set STA - mobility
                switch(this->typeMob){
                    case __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_STATIC:
                        mobSTAs.SetMobilityModel("ns3::ConstantPositionMobilityModel");
                        break;
                    case __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_UNIFORM_WALK:
                        mobSTAs.SetMobilityModel("ns3::ConstantVelocityMobilityModel", "Velocity", VectorValue(Vector(this->staFixedSpeedX,this->staFixedSpeedY, 0)));
                        break;
                    case __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_WAYPOINT:
                        mobSTAs.SetMobilityModel("ns3::RandomWaypointMobilityModel", "Speed", PointerValue (speedRange),"Pause", PointerValue (pauseRange),"PositionAllocator", PointerValue(posSTAsRandom));
                        break;
                    case __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_WALK:
                        break;
                    case __SDN_LAB_SYSTEMMODEL_MOBILITY_TYPE_RANDOM_DIRECTION:
                        break;
                }
                mobSTAs.Install(*this->stas);
            }
            
        };
    }
#endif
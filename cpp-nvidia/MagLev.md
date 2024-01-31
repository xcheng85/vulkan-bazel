# Nvidia MagLev project

## overview

1. Perception: 2D/3D detection, segmetation, tracking and (early/late) fusion etc.

2. Mapping-Localization: semantic map, feature design, map update/online mapping, SLAM, pose estimation and odometry etc.

3. Prediction: trajectory forecasting, agent behavior & interaction, multimodal, and perception-prediction etc.

4. Planning: reinforcement learning, imitation learning, inverse reinforcement learning, localization & personalization of planning (aggressive or conservative), prediction-planning, and mapping-localization-prediction-planning etc.

5. Control: reinforcement learning, imitation learning, inverse reinforcement learning, and planning-control etc.

6. Sensor Data Preprocessing: pollution/dust detection, defogging, deraining, desnowing, denoising, and enhancement etc.

7. Simulation: vehicle/human, sensor, traffic, road and envirnment modeling etc.

## Component
Data driven models for autonomous driving;

*Cloud computing infrastructure and big data processing;

*Annotation tools for training data;

*Large scale model training platform;

*Model testing and verification;

*Related machine learning techniques.

## Cloud computing infrastructure and big data processing;

big data processing tools in cloud service: Presto, Apache Spark/Flint, Apache Kafka, Apahce Cassandra, Apche Hbase, Apache Hudi, Apache Airflow, Apache Mesos and Kubernetes


AWS s3

“PointPillars: Fast Encoders for Object Detection from Point Clouds”

“Joint 3D Proposal Generation and Object Detection from View Aggregation”

end-to-end workflows — including data preprocessing, selection, model training, testing and pruning

lidar and radar sensors and the entire dataset for training over these millions of miles amounts to hundreds of petabytes.

data processing necessary to train and validate industry-grade AI systems 


## use case for visualization
voxelized lidar

rasterization representation in the mapping step

visualization tools are used for viewing/debugging/replaying the data, besides of annotation. 

Uber gives a open sourced visualization tool, Autonomous Visualization System (AVS): avs.auto

XVIZ is the Protocol for Real-Time Transfer and Visualization of Autonomy Data

grpc 

iot sent raw data to server

server and client(monitor inside the av) bidirectional stream api: decode, buffer and 3d rendering

react component

streetscape.gl: based on webgl

## deploy model to av

Every new model version we deploy to our self-driving car entails a journey through hundreds of petabytes of collected data, millions of labeled frames, dozens of dataset versions, and hundreds of experiments, each consuming days of training.

## production
production engineers to seamlessly deploy models to the car.

## observability
logging each model’s accuracy and performance,
Metrics Traceability

metrics server storage, scalability

## relationship with k8s
 deploy GPU-accelerated inference across Kubernetes clusters. The combination of NVIDIA TensorRT inference server and Kubeflow makes data

## visualization part
Dashboard for MagLev experience, visualizing results, spinning up notebooks, sharing pipelines, data exploration / browsing

## Realtime be processing

### Examples of streams
1. Projected lidar stream
2. cameras
3. radar

### workflow
directed graph of jobs. 

Workflow Infra/Services: grpc 

Workflow Traceability

Read/Stream/Write data
for DL/ML apps

### How data captured in av to cloud
In-house “TransferPilot” systems automate upload
from SSD to S3


### workflow and k8s
aws argocd

### ETL
Query Engines Cloud

Spark ETL jobs parse data, index key metadata, and
create Parquet files query-able by Presto database
• Custom clients and general SQL interface allow
developers access to full dataset


### 3D + Labeling
1500 human: labelers

All done in HumanLoop, an web-based platform

Bounding boxes (and cuboids)
• Instance segmentation
• Polyline annotations
• Object tracking in videos
• Hierarchical classification


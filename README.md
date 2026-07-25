# OpenStage
Library that manages the stages of a graphics pipeline, allowing modularity over handles rather than creating fixed pipelines. The idea is to abstract what happens on those handles and simply passing config-like information to a stager that mounts pipelines easily, so that the core work of testing a certain effect is minimized to the bare minimum.

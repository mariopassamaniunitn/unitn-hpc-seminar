import torch

print ("CUDA libraries are {0}".format("available" if torch.cuda.is_available() else "not available"))

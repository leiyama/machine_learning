import matplotlib.pyplot as plt
import numpy as np

def main():
	
	k = [10, 20, 30, 50, 100]
	Error_0 = np.array([ \
	[30388.3532411 , 37760.560158],\
	[29120.4493743 , 45870.582089],\
	[7825.24844576 , 50484.0257657],\
	[1148.30623157 , 50936.018838],\
	[22.3181465509 , 32344.4113227]\
	])

	Error_1 = np.array([ \
	[31573.8806693 , 33075.6020007],\
	[9303.2374383 ,35801.9750589 ],\
	[8338.19376462 , 39494.3030191],\
	[1168.31845325 , 41028.0871932],\
	[48.3379809485 , 34916.3515934]\
	])
    
    # plot the in error and out error
    # without and with regularization
	plt.figure(1)
	plt.plot(k, Error_0[:, 0],'r.-')
	plt.plot(k, Error_0[:, 1])
	

	plt.figure(2)
	plt.plot(k, Error_1[:, 0],'r.-')
	plt.plot(k, Error_1[:, 1])
	plt.show()

if __name__ == '__main__':
	main()

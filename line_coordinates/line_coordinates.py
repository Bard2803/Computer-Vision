# Import neccessery libraries
import matplotlib.pyplot as plt
import numpy as np
from tensorflow import keras
from keras import layers
from PIL import Image
from sklearn.model_selection import train_test_split

def save_plot(n):
    ''' Prints the line images in matplotlib and saves them on the disk.
        Also returns the start and end coordinates of the line.'''

    points_list = []
    # Generate chosen number of images
    for i in range(n):
        # Set the resolution of images. Images should be 64x64
        resolution = 64
        # Set the properties to print 64x64 pixels images
        # Has to be set before the objects instatiation!
        plt.rcParams["figure.figsize"] = [resolution/100, resolution/100]
        # Instatiate figure and axes objects. 
        fig, axes = plt.subplots()
        # Set limits for the axes so each is in the range [0, 64]
        axes.set(xlim=(0, resolution), ylim=(0, resolution))
        # Randomly generate the start and end coordinates of the line
        points = np.random.randint(resolution, size=(4))
        points_list.append(points)
        x = [points[0], points[2]]
        y = [points[1], points[3]]
        # Get rid of all the axes, so only black line on white background is generated
        plt.gca().set_axis_off()
        axes.plot(x, y, color="black")
        #Save line images and enumerate their name
        plt.savefig(f"line_images/line{i}.png")
        # Block the printing in the jupyter notebook to avoid cluttering the view
        plt.close()
    # Return the start and end coordinates of lines for further use
    return points_list


def load_images(n):
    '''Loads the images and converts them to numpy arrays of pixels in grayscale.'''

    image_list = []
    for i in range(n): 
        # Convert the 4 channels of the images into 1 channel (grayscale) to decrease the number of features
        im=Image.open(f"line_images/line{i}.png").convert('L')
        image_list.append(np.asarray(im))
    return np.asarray(image_list)


def datasets_creation(x, y):
    ''' Creates training and test datasets and normalizes them.
        For x set the pixels have to be in range [0,1].
        For y set there are four numbers for each data point (image) 
        which are in the form y_train = [x_start, y_start, x_end, y_end]'''

    x_train = (x/255).reshape(x.shape[0], x.shape[1], x.shape[2], 1)
    y_train = np.asarray(y)
    x_train, x_test, y_train, y_test = train_test_split(x_train, y_train , random_state=104, test_size=0.2, shuffle=True)
    return x_train, x_test, y_train, y_test


def get_model(input_shape, output_shape):
    ''' Constructs the model with appropriate input and output shapes '''

    model = keras.Sequential(
        [
            keras.Input(shape=input_shape),
            layers.Conv2D(32, kernel_size=(3, 3), activation="relu", padding="same", strides=1),
            layers.MaxPooling2D(pool_size=(2, 2)),
            layers.Conv2D(64, kernel_size=(3, 3), activation="relu", padding="same", strides=1),
            layers.MaxPooling2D(pool_size=(2, 2)),
            layers.Flatten(),
            layers.Dropout(0.3),
            layers.Dense(output_shape, activation="relu"),
        ]
    )
    # Print model summary to verify its structure
    model.summary()
    return model


def MAE(y_test, y_pred):
    ''' Calculate the MAE on test set'''

    return round(np.sum(abs(y_test-y_pred))/y_test.size, 4)



if __name__ == "__main__":
    # Number of images to generate.
    n=1000
    coordinates = save_plot(n)
    image_list = load_images(n)
    x_train, x_test, y_train, y_test = datasets_creation(image_list, coordinates)
    # First dimension in the datasets is the number of datapoints, 
    # so it is ommited in the shapes passed to the function
    model = get_model(x_train.shape[1:], y_train.shape[1:][0])
    model.compile(loss='mae', optimizer='adam')
    model.fit(x_train, y_train, batch_size=50, epochs=50, validation_split=0.1)
    y_pred = model.predict(x_test, batch_size = 50)
    print(f"The MAE on test set is: {MAE(y_test, y_pred)}")
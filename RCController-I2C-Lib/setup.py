import setuptools

setuptools.setup(
    name="rccontroller-i2c-lib",
    version="1.0.0",
    author="Marcel Egle",
    author_email="info@marcel-egle.de",
    description="RCController I2C-Master Library",
    url="https://github.com/check2016",
    packages=['rccontroller'],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)
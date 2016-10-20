from setuptools import setup, Extension

ext1 = Extension(
    'spatial_index', ['spatial.cpp'],
    libraries=['boost_python'],
    extra_compile_args=['-std=c++0x', '-g'],
)


setup(
    name='spatial_index',
    version='0.0.1',
    author='Maxim Novikov',
    author_email='nes.8bit@yandex.ru',
    license='MIT',
    install_requires=[''],
    classifiers=['Development Status :: 3 - Alpha'],
    ext_modules=[ext1],
)

from invoke import task


@task
def check(c):
    c.run('clang-tidy populous_simulacra.cpp person.hpp')


@task
def format(c):
    c.run('clang-format -i populous_simulacra.cpp person.hpp')


@task(pre=[format, check], default=True)
def default(c):
    pass


@task(pre=[format])
def build(c):
    c.run('g++ -static populous_simulacra.cpp')

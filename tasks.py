from invoke import task


@task
def check(c):
    c.run('clang-tidy populous_simulacra.cpp')


@task
def format(c):
    c.run('clang-format -i populous_simulacra.cpp')


@task(pre=[format, check], default=True)
def default(c):
    pass

#!/usr/bin/python3
import os


def options(opt):
    opt.load('compiler_cxx waf_unit_test')


def configure(cfg):
    cfg.load('compiler_cxx waf_unit_test')


def build(bld):
    bld.add_group()
    for root, dirs, files in os.walk('test'):
        for filename in files:
            if os.path.splitext(filename)[1] == ".cpp":
                bld.program(
                    features='test',
                    source=os.path.join(root, filename),
                    target=filename + '.test',
                    includes='.',
                    cxxflags=['-std=c++1z', '-Wall', '-Werror'],
                )
    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)

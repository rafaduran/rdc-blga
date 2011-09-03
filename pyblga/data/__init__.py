from __future__ import unicode_literals
from __future__ import absolute_import
from __future__ import print_function
from future_builtins import * #@UnusedWildImport pylint:disable=W0614
import logging

from sqlalchemy import create_engine
from sqlalchemy.orm import joinedload, aliased, sessionmaker
from sqlalchemy.pool import StaticPool

import data.base_models as bases 

_ENGINE = None
_MAKER = None
BASE = bases.Base


OPTIONS = {'debug': True, 
           'verbose': False, 
           'timeout': 3600,
           'sql_connection': 'sqlite:///blga.sqlite3',
           'models': ['Runs', 'Parameters', 'Results', 'RunsParamsAssoc', 
                      'Searcher'] 
           }

def configure_backend(options=OPTIONS):
    """
    Establish the database, create an engine if needed, and
    register the models.

    Args:
        options. Dictionary of configuration options
    """
    global _ENGINE
    if not _ENGINE:
        debug = options['debug']
        verbose = options['verbose']
        timeout = options['timeout']

        _ENGINE = create_engine(options['sql_connection'], pool_recycle=timeout)

        logger = logging.getLogger('sqlalchemy.engine')
        if debug:
            logger.setLevel(logging.DEBUG)
        elif verbose:
            logger.setLevel(logging.INFO)

        register_models(options)


def get_session(autocommit=True, expire_on_commit=False):
    """Helper method to grab session"""
    global _MAKER, _ENGINE
    if not _MAKER:
        assert _ENGINE
        _MAKER = sessionmaker(bind=_ENGINE,
                              autocommit=autocommit,
                              expire_on_commit=expire_on_commit)
    return _MAKER()


def register_models(options):
    """Register Models and create properties"""
    global _ENGINE
    assert _ENGINE
    model_objects = options['models']
    tables = []
    for model in model_objects:
        try:
            model_class = getattr(bases, model)
            tables.append(model_class.__table__)
        except AttributeError as err:
            logging.error("Unknown model: {0}".format(model))
            raise err
    BASE.metadata.create_all(_ENGINE, tables=tables, checkfirst=True)


def unregister_models():
    """Unregister Models, useful clearing out data before testing"""
    global _ENGINE
    assert _ENGINE
    BASE.metadata.drop_all(_ENGINE)
    
configure_backend()
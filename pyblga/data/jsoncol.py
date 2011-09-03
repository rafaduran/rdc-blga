#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`JSONCol` --- Very short description
=======================================
:py:class:`JSONCol` long description

Taken `from <http://www.sqlalchemy.org/trac/wiki/UsageRecipes/JSONColumn>`_ and
changed to fit my needs.
"""

from sqlalchemy import types
import copy
import json

json_null = object()

class JSONCol(types.MutableType, types.TypeDecorator):
    impl = types.Unicode

    def process_bind_param(self, value, dialect):
        if value is json_null:
            value = None
        return json.dumps(value)

    def process_result_value(self, value, dialect):
        if value is None:
            return None
        return json.loads(value)

    def copy_value(self, value):
        return copy.deepcopy(value)


if __name__ == '__main__':
    from sqlalchemy import * #@UnusedWildImport
    engine = create_engine('sqlite://', echo=True)

    m = MetaData(engine)

    t1 = Table('table', m, 
        Column('data', JSONCol())
    )

    m.create_all()


    t1.insert().execute(
        {'data':"a string"},
        {'data':{"a":"dict"}},
        {'data':None},
    )

    print t1.select().where(t1.c.data==json_null).execute().fetchall()
    print t1.select().where(t1.c.data!=json_null).execute().fetchall()
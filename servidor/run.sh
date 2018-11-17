#!/bin/bash
virtualenv venv
source venv/bin/activate
export FLASK_APP=__init__.py
export FLASK_ENV=development
flask run
